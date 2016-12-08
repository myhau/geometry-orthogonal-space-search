#include "gogui.hpp"

int main()
{
  gogui::vector<gogui::Point> cloud;
  gogui::Point p1(2,3);
  gogui::Point p2(3,4);
  gogui::Point p3(1,4);

  cloud.push_back(p1);
  gogui::snapshot();

  cloud.push_back(p2);
  gogui::snapshot();
  
  cloud.push_back(p3);
  gogui::snapshot();


  gogui::vector<gogui::Line> lines;

  gogui::Line line1(p1, p2);
  lines.push_back(line1);
  gogui::snapshot();
  
  {
    gogui::ActiveLine active(line1);
    gogui::snapshot();
  }
  gogui::snapshot();
  gogui::printJSON();
}
