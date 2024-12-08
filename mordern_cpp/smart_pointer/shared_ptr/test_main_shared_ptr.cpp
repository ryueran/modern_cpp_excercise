#include <gtest/gtest.h>
#include <memory>
#include <vector>
#include <algorithm>
#include "Widget.hh"

// Include the Widget class definition here or in an included header file.

class WidgetTest : public ::testing::Test {
protected:
    // Helper function to create a widget.
    std::shared_ptr<Widget> createWidget(int height, int width) {
        return Widget::create(height, width);
    }
};

// Test creation of Widget
TEST_F(WidgetTest, CreateWidget) {
    auto widget = createWidget(10, 20);
    ASSERT_NE(widget, nullptr) << "Widget should be successfully created.";
}

// Test shared_from_this functionality
TEST_F(WidgetTest, SharedFromThis) {
    auto widget = createWidget(10, 20);
    widget->process(); // Calls shared_from_this internally.

    // Verifying that the process_widgets vector contains the widget.
    auto weak_ptr = widget->read_process_widget().front();
    ASSERT_FALSE(weak_ptr.expired()) << "Weak pointer should not expire.";
    auto shared_widget = weak_ptr.lock();
    ASSERT_EQ(shared_widget, widget) << "Shared pointer should match the original widget.";
}

// Test multiple Widgets processing
TEST_F(WidgetTest, MultipleWidgetsProcessing) {
    auto widget1 = createWidget(10, 20);
    auto widget2 = createWidget(30, 40);

    widget1->process();
    widget2->process();

    // Check if each widget's process_widgets contains itself correctly.
    ASSERT_EQ(widget1->read_process_widget().size(), 1);
    ASSERT_EQ(widget2->read_process_widget().size(), 1);

    auto weak_ptr1 = widget1->read_process_widget().front();
    auto weak_ptr2 = widget2->read_process_widget().front();

    ASSERT_EQ(weak_ptr1.lock(), widget1);
    ASSERT_EQ(weak_ptr2.lock(), widget2);
}
