#ifndef ROBOT_HPP
#define ROBOT_HPP
#include <algorithm>

namespace muhamadiarov
{
  struct Robot {
    Robot():
      maxEnergy_(100.0),
      recoveryRate_(1.0),
      speed_(5.0),
      currentEnergy_(100.0)
    {}

    Robot(double e, double r, double s):
      maxEnergy_(e),
      recoveryRate_(r),
      speed_(s),
      currentEnergy_(e) {}

    void resetEnergy()
    {
      currentEnergy_ = maxEnergy_;
    }

    void charge()
    {
      currentEnergy_ = std::min(maxEnergy_, currentEnergy_ + recoveryRate_);
    }

    bool consumeEnergy(double amount)
    {
      if (currentEnergy_ >= amount)
      {
        currentEnergy_ -= amount;
        return true;
      }
      return false;
    }

    double getMaxEnergy() const
    {
      return maxEnergy_;
    }
    double getRecoveryRate() const
    {
      return recoveryRate_;
    }
    double getSpeed() const
    {
      return speed_;
    }
    double getCurrentEnergy() const
    {
      return currentEnergy_;
    }

    void setMaxEnergy(double e)
    {
      maxEnergy_ = e;
      currentEnergy_ = std::min(currentEnergy_, maxEnergy_);
    }

    void setRecoveryRate(double r)
    {
      recoveryRate_ = r;
    }
    void setSpeed(double s)
    {
      speed_ = s;
    }
  private:
    double maxEnergy_;
    double recoveryRate_;
    double speed_;
    double currentEnergy_;
  };
}

#endif
