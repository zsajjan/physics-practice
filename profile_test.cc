#include "gtest/gtest.h"
#include "profile.h"
#include <fstream>

constexpr double kDt = 0.005;
const double kEpsilon = 1e-9;

TEST(Profile, Test) {
  // params
  const Profile::ProfilePoint current = {0., 0.};
  const Profile::ProfilePoint goal = {2., 0.};

  Profile profile(current);
  profile.SetGoal(goal);

  double t = profile.GetTime();

  Profile::ProfilePoint prev_point;
  Profile::ProfilePoint point = profile.GetPoint(0.0);

  std::ofstream csv("/tmp/profile.csv");

  for (double i = 0; i < t; i += kDt) {
    prev_point = point;
    point = profile.GetPoint(i);
    EXPECT_LE(point.velocity, kMaxVelocity);
    EXPECT_LE(point.velocity - prev_point.velocity, kMaxAcceleration * kDt + kEpsilon);

    csv << i << "," << point.position << "," << point.velocity << "\n";
  }

  EXPECT_NEAR(profile.GetPoint(t).position, goal.position, kEpsilon);
  EXPECT_NEAR(profile.GetPoint(t).velocity, goal.velocity, kEpsilon);
}
