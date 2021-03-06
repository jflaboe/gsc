#include "Base_eval_item_widget.h"
#include "Evaluation_view.h"
#include "Explanation_view_widget.h"
#include "../model/Eval_item.h"
#include "../model/Session.h"

#include <Wt/WPushButton>
#include <Wt/WTemplate>
#include <Wt/WText>

Base_eval_item_widget::Base_eval_item_widget(const Submission::Item& model,
                                             Evaluation_view& main,
                                             Session& session,
                                             Wt::WContainerWidget* parent)
        : WContainerWidget(parent),
          model_(model),
          main_(main),
          session_(session)
{ }

void Base_eval_item_widget::retract_action_()
{
    if (!main_.can_eval()) return;

    if (model_.self_eval) {
        dbo::Transaction transaction(session_);
        Submission::retract_self_eval(model_.self_eval);
        transaction.commit();

        main_.go_default();
    }
}

void Base_eval_item_widget::add_item_heading_()
{
    auto h4 = new Wt::WTemplate("<h4>Question ${number} "
                                        "<small>(${value})</small></h4>",
                                this);

    std::string number = boost::lexical_cast<std::string>(
            model_.eval_item->sequence());
    std::string value = Eval_item::pct_string(
            model_.eval_item->relative_value()
            / main_.submission()->point_value());

    h4->bindWidget("number", new Wt::WText(number));
    h4->bindWidget("value", new Wt::WText(value));
}

void Base_eval_item_widget::add_question_()
{
    auto p = new Wt::WTemplate("<p class='question'>${question}</p>", this);
    p->bindWidget("question", new Wt::WText(model_.eval_item->prompt()));
}

void Base_eval_item_widget::add_evaluation_(const std::string& heading,
                                            const std::string& score,
                                            const std::string& explanation,
                                            const std::string& highlight_style)
{
    auto p = new Wt::WTemplate(
            "<h5>${heading}</h5>"
            "<p class='answer'>"
              "<strong>${score}.</strong>"
              " ${explanation}"
            "</p>",
            this);

    p->bindWidget("heading", new Wt::WText(heading));
    p->bindWidget("score", new Wt::WText(score));
    p->bindWidget("explanation",
                  new Explanation_view_widget(explanation,
                                              main_.file_viewer(),
                                              highlight_style));
}

void Base_eval_item_widget::add_navigation_(bool focus)
{
    auto buttons = new Wt::WContainerWidget(this);
    buttons->setStyleClass("buttons");

    auto prev_btn = new Wt::WPushButton("Prev", buttons);
    auto list_btn = new Wt::WPushButton("List", buttons);
    auto next_btn = new Wt::WPushButton("Next", buttons);

    auto sequence = model_.eval_item->sequence();

    if (sequence > 1) {
        prev_btn->clicked().connect(std::bind([=]() {
            main_.go_to((unsigned) (sequence - 1));
        }));
    } else prev_btn->disable();

    list_btn->setFocus(focus);
    list_btn->clicked().connect(std::bind([=]() { main_.go_default(); }));

    if (sequence < main_.submission()->item_count()) {
        next_btn->clicked().connect(std::bind([=]() {
            main_.go_to((unsigned) (sequence + 1));
        }));
    } else next_btn->disable();
}

