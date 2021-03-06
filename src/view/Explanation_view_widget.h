#pragma once

#include <Wt/WCompositeWidget>
#include <Wt/WContainerWidget>

#include <string>

class File_viewer_widget;

class Explanation_view_widget : public Wt::WCompositeWidget {
public:
    Explanation_view_widget(const std::string& content,
                            Wt::WContainerWidget* parent = nullptr);

    Explanation_view_widget(const std::string& content,
                            File_viewer_widget* viewer,
                            const std::string& highlight_style,
                            Wt::WContainerWidget* parent = nullptr);

private:
    File_viewer_widget* const viewer_;
    const std::string highlight_style_;

    void initialize_viewer_(const std::string& content);
};