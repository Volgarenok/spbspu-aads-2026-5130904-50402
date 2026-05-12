#include <sstream>
#include <string>
#include <boost/test/unit_test.hpp>
#include "gameTypes.hpp"
#include "serialization.hpp"

using namespace karpovich;

BOOST_AUTO_TEST_SUITE(SerializationTests)

BOOST_AUTO_TEST_CASE(test_item_roundtrip)
{
  item_template_t original{"key", "INVENTORY", "Rusty key"};
  std::ostringstream oss;
  serializeItem(original, oss);
  std::istringstream iss(oss.str());
  item_template_t loaded = deserializeItem(iss);
  BOOST_CHECK_EQUAL(loaded.key_, "key");
  BOOST_CHECK_EQUAL(loaded.type_, "INVENTORY");
  BOOST_CHECK_EQUAL(loaded.name_, "Rusty key");
}

BOOST_AUTO_TEST_CASE(test_link_roundtrip)
{
  scene_link_t original{"castle", "Go to castle", "has:key"};
  std::ostringstream oss;
  serializeLink(original, oss);
  std::istringstream iss(oss.str());
  scene_link_t loaded = deserializeLink(iss);
  BOOST_CHECK_EQUAL(loaded.target_, "castle");
  BOOST_CHECK_EQUAL(loaded.description_, "Go to castle");
  BOOST_CHECK_EQUAL(loaded.condition_, "has:key");
}

BOOST_AUTO_TEST_CASE(test_object_roundtrip)
{
  scene_object_t original{"chest", "open", "gold", "has:key"};
  std::ostringstream oss;
  serializeObject(original, oss);
  std::istringstream iss(oss.str());
  scene_object_t loaded = deserializeObject(iss);
  BOOST_CHECK_EQUAL(loaded.key_, "chest");
  BOOST_CHECK_EQUAL(loaded.action_, "open");
  BOOST_CHECK_EQUAL(loaded.gives_, "gold");
  BOOST_CHECK_EQUAL(loaded.requires_, "has:key");
}

BOOST_AUTO_TEST_CASE(test_scene_roundtrip)
{
  scene_t original;
  original.id_ = "tavern";
  original.description_ = "You are in a tavern";
  original.links_.pushBack(scene_link_t{"forest", "Go to forest", ""});
  original.objects_.pushBack(scene_object_t{"sword", "take", "", ""});

  std::ostringstream oss;
  serializeScene(original, oss);
  std::istringstream iss(oss.str());
  scene_t loaded = deserializeScene(iss);

  BOOST_CHECK_EQUAL(loaded.id_, "tavern");
  BOOST_CHECK_EQUAL(loaded.description_, "You are in a tavern");
  BOOST_CHECK_EQUAL(loaded.links_.getSize(), 1);
  BOOST_CHECK_EQUAL(loaded.objects_.getSize(), 1);
  BOOST_CHECK_EQUAL(loaded.links_[0].target_, "forest");
  BOOST_CHECK_EQUAL(loaded.objects_[0].key_, "sword");
}

BOOST_AUTO_TEST_CASE(test_project_roundtrip)
{
  project_t original;
  original.title_ = "TestQuest";
  original.start_scene_id_ = "start";
  scene_t s;
  s.id_ = "start";
  s.description_ = "Start room";
  original.scenes_.add("start", std::move(s));

  std::ostringstream oss;
  serializeProject(original, oss);
  std::istringstream iss(oss.str());
  project_t loaded = deserializeProject(iss);

  BOOST_CHECK_EQUAL(loaded.title_, "TestQuest");
  BOOST_CHECK(loaded.scenes_.has("start"));
  BOOST_CHECK_EQUAL(loaded.start_scene_id_, "start");
}

BOOST_AUTO_TEST_CASE(test_save_roundtrip)
{
  save_state_t original;
  original.current_scene_id_ = "castle";
  original.inventory_.add("gold", 5);
  original.inventory_.add("key", 1);
  original.running_ = true;

  std::ostringstream oss;
  serializeSave(original, oss);
  std::istringstream iss(oss.str());
  save_state_t loaded = deserializeSave(iss);

  BOOST_CHECK_EQUAL(loaded.current_scene_id_, "castle");
  BOOST_CHECK_EQUAL(loaded.inventory_.get("gold"), 5);
  BOOST_CHECK_EQUAL(loaded.inventory_.get("key"), 1);
  BOOST_CHECK_EQUAL(loaded.running_, true);
}

BOOST_AUTO_TEST_SUITE_END()
