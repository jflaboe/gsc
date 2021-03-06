#include "Review_eval_item_widget.h"
#include "../model/Grader_eval.h"
#include "../model/Self_eval.h"

#include <Wt/WText>

Review_eval_item_widget::Review_eval_item_widget(
        const Submission::Item& model,
        Evaluation_view& main,
        Session& session,
        Wt::WContainerWidget* parent)
        : Single_eval_item_widget(model, main, session, parent)
{
    auto& eval_item = model.eval_item;
    auto& self_eval = model.self_eval;
    auto& grader_eval = model.grader_eval;

    if (self_eval) {
        add_evaluation_("Self evaluation",
                        eval_item->format_score(self_eval->score()),
                        self_eval->explanation());

        if (grader_eval && self_eval->submission()->is_graded())
        {
            add_evaluation_("Grader evaluation",
                            eval_item->format_score(grader_eval->score()),
                            grader_eval->explanation(),
                            "grader-highlight");
        }
    } else {
        new Wt::WText("<h5>No self evaluation submitted!</h5>", this);
    }

    add_navigation_();
}

