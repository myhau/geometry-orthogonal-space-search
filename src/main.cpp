#include <iostream>
#include <catch.hpp>
#include "RangeSearchTree2D.h"
#include "util/Functions.h"

using namespace std;

struct InputData {
  Point<> p;
  string data;

  InputData(const Point<> &p, const string &data) : p(p), data(data) {

  }

  bool operator<(const InputData &rhs) const {
    return data < rhs.data;
  }

  bool operator>(const InputData &rhs) const {
    return rhs < *this;
  }

  bool operator<=(const InputData &rhs) const {
    return !(rhs < *this);
  }

  bool operator>=(const InputData &rhs) const {
    return !(*this < rhs);
  }

  InputData() {}
};

vector<double> split(string s, string delimiter) {
  size_t pos = 0;
  string token;
  vector<double> tokens;
  while ((pos = s.find(delimiter)) != string::npos) {
    token = s.substr(0, pos);
    tokens.push_back(stod(token));
    s.erase(0, pos + delimiter.length());
  }
  tokens.push_back(stod(s));
  return tokens;
}


int main() {

  int t;
  cin >> t;

  vector<InputData> points;

  for (int i = 0; i < t; ++i) {
    double x, y;
    string label;
    cin >> x >> y >> label;
    points.push_back(InputData(Point<>(x, y), label));
  }

  auto searchTree = RangeSearchTree2D<InputData>(points, [](const InputData& data){return data.p;});


  while(true) {
    cout << "Query tree, format:x1,x2,y1,y2. End with quit" << endl;
    string query;
    cin >> query;

    if(query == "quit") break;

    vector<double> params = split(query, ",");

    set<InputData> results = searchTree.search(params[0], params[1], params[2], params[3]);

    cout << "Results: " << endl;

    for (auto &&result : results) {
      cout << "p=(" << result.p.x << "," << result.p.y << "), d=" << result.data << endl;
    }

    cout << endl;
  }



  return 0;
}