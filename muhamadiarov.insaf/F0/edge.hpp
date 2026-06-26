#ifndef EDGE_HPP
#define EDGE_HPP
#include <string>

namespace muhamadiarov
{
  enum class RoadType
  {
    ASPHALT,
    GROUND,
    SWAMP
  };

  const double energyMultiplier[] = {1.0, 1.5, 2.5};
  const double timeMultiplier[] = {1.0, 1.5, 3.0};

  inline RoadType charToRoadType(char c)
  {
    switch (c)
    {
      case 'a': return RoadType::ASPHALT;
      case 'g': return RoadType::GROUND;
      case 's': return RoadType::SWAMP;
      default: return RoadType::ASPHALT;
    }
  }

  inline char roadTypeToChar(RoadType t)
  {
    switch (t)
    {
      case RoadType::ASPHALT: return 'a';
      case RoadType::GROUND: return 'g';
      case RoadType::SWAMP: return 's';
      default: return 'a';
    }
  }

  inline std::string roadTypeToString(RoadType t)
  {
    switch (t)
    {
      case RoadType::ASPHALT: return "asphalt";
      case RoadType::GROUND: return "ground";
      case RoadType::SWAMP: return "swamp";
      default: return "asphalt";
    }
  }

  struct Edge
  {
    int to_;
    RoadType type_;
    size_t distance_;

    Edge():
      to_(-1),
      type_(),
      distance_(0)
    {}

    Edge(int t, RoadType rt, size_t d):
      to_(t),
      type_(rt),
      distance_(d)
    {}

    double getTime(double speed) const
    {
      double multiplier = timeMultiplier[static_cast<int>(type_)];
      return static_cast<double>(distance_) / speed * multiplier;
    }

    double getEnergy(double speed) const
    {
      double multiplier = energyMultiplier[static_cast<int>(type_)];
      return static_cast<double>(distance_) * multiplier / speed;
    }

    bool operator==(const Edge& other) const
    {
      return to_ == other.to_ && type_ == other.type_ && distance_ == other.distance_;
    }

    bool operator!=(const Edge& other) const
    {
      return !(*this == other);
    }
  };
}

#endif
