#include <iostream>
#include <sstream>
#include <boost/test/unit_test.hpp>
#include "cmds.hpp"
#include "player.hpp"

using namespace dirko;

BOOST_AUTO_TEST_SUITE(PlayerTests)
BOOST_AUTO_TEST_CASE(test_add)
{
  save_t saver;
  pls_t db;
  db.add("All", pl_t{});
  pls_iter_t pl = db.begin();
  pl_iter_t tr = db.get("All").begin();
  std::istringstream in("./dirko.nikita/F0/tr1.wav t1");
  std::ostringstream dump("");
  add(in, dump, db, tr, pl, saver);
  BOOST_CHECK(db.get("All").has("t1"));
  BOOST_CHECK(saver.has("t1"));
}
BOOST_AUTO_TEST_CASE(test_remove)
{
  save_t saver;
  pls_t db;
  db.add("All", pl_t{});
  pls_iter_t pl = db.begin();
  pl_iter_t tr = db.get("All").begin();
  std::istringstream in("./dirko.nikita/F0/tr1.wav t1");
  std::ostringstream dump("");
  add(in, dump, db, tr, pl, saver);
  in.str("t1");
  in.clear();
  remove(in, dump, db, tr, pl, saver);
  BOOST_CHECK(!db.get("All").has("t1"));
  BOOST_CHECK(!saver.has("t1"));
}
BOOST_AUTO_TEST_CASE(test_rename)
{
  save_t saver;
  pls_t db;
  db.add("All", pl_t{});
  pls_iter_t pl = db.begin();
  pl_iter_t tr = db.get("All").begin();
  std::istringstream in("./dirko.nikita/F0/tr1.wav t1 t1 rename");
  std::ostringstream dump("");
  add(in, dump, db, tr, pl, saver);
  rename(in, dump, db, tr, pl, saver);
  BOOST_CHECK(!db.get("All").has("t1"));
  BOOST_CHECK(db.get("All").has("rename"));
}
BOOST_AUTO_TEST_CASE(test_play)
{
  save_t saver;
  pls_t db;
  db.add("All", pl_t{});
  pls_iter_t pl = db.begin();
  pl_iter_t tr = db.get("All").begin();
  std::ostringstream out("");
  std::istringstream in("./dirko.nikita/F0/tr2.wav t2 t2 1 t2 200");
  std::ostringstream dump("");
  add(in, dump, db, tr, pl, saver);
  play(in, out, db, tr, pl, saver);
  BOOST_CHECK(out.str() == "<duration: 6.24327>\n<PLAYING: t2 until 1>\n<DONE>");
  out.str("");
  play(in, out, db, tr, pl, saver);
  BOOST_CHECK(out.str() == "<duration: 6.24327>\n<PLAYING: t2 until 6.24327>\n<DONE>");
}
BOOST_AUTO_TEST_CASE(test_next)
{
  save_t saver;
  pls_t db;
  db.add("All", pl_t{});
  pls_iter_t pl = db.begin();
  pl_iter_t tr = db.get("All").begin();
  std::ostringstream out("");
  std::istringstream in("./dirko.nikita/F0/tr1.wav t1 ./dirko.nikita/F0/tr2.wav t2 1");
  std::ostringstream dump("");
  add(in, dump, db, tr, pl, saver);
  add(in, dump, db, tr, pl, saver);
  next(in, out, db, tr, pl, saver);
  BOOST_CHECK(tr == ++(db.get("All").begin()));
}
BOOST_AUTO_TEST_CASE(test_prev)
{
  save_t saver;
  pls_t db;
  db.add("All", pl_t{});
  pls_iter_t pl = db.begin();
  pl_iter_t tr = db.get("All").begin();
  std::ostringstream out("");
  std::istringstream in("./dirko.nikita/F0/tr1.wav t1 ./dirko.nikita/F0/tr2.wav t2 1");
  std::ostringstream dump("");
  add(in, dump, db, tr, pl, saver);
  add(in, dump, db, tr, pl, saver);
  prev(in, out, db, tr, pl, saver);
  BOOST_CHECK(tr == --(db.get("All").end()));
}
BOOST_AUTO_TEST_CASE(test_list)
{
  save_t saver;
  pls_t db;
  db.add("All", pl_t{});
  pls_iter_t pl = db.begin();
  pl_iter_t tr = db.get("All").begin();
  std::ostringstream out("");
  std::istringstream in("./dirko.nikita/F0/tr1.wav t1 ./dirko.nikita/F0/tr2.wav t2");
  std::ostringstream dump("");
  add(in, dump, db, tr, pl, saver);
  add(in, dump, db, tr, pl, saver);
  list(in, out, db, tr, pl, saver);
  BOOST_CHECK(out.str() == "<All\nt1\nt2>");
}
BOOST_AUTO_TEST_CASE(test_pl_add_remove_and_list)
{
  save_t saver;
  pls_t db;
  db.add("All", pl_t{});
  pls_iter_t pl = db.begin();
  pl_iter_t tr = db.get("All").begin();
  std::ostringstream out("");
  std::istringstream in("playlist playlist");
  std::ostringstream dump("");
  pl_add(in, dump, db, tr, pl, saver);
  pl_list(in, out, db, tr, pl, saver);
  BOOST_CHECK(out.str() == "<PLAYLISTS\nAll\nplaylist>");
  out.str("");
  pl_remove(in, dump, db, tr, pl, saver);
  pl_list(in, out, db, tr, pl, saver);
  BOOST_CHECK(out.str() == "<PLAYLISTS\nAll>");
}
BOOST_AUTO_TEST_CASE(test_pl_rename)
{
  save_t saver;
  pls_t db;
  db.add("All", pl_t{});
  pls_iter_t pl = db.begin();
  pl_iter_t tr = db.get("All").begin();
  std::ostringstream out("");
  std::istringstream in("playlist playlist p");
  std::ostringstream dump("");
  pl_add(in, dump, db, tr, pl, saver);
  pl_list(in, out, db, tr, pl, saver);
  BOOST_CHECK(out.str() == "<PLAYLISTS\nAll\nplaylist>");
  out.str("");
  pl_rename(in, dump, db, tr, pl, saver);
  pl_list(in, out, db, tr, pl, saver);
  BOOST_CHECK(out.str() == "<PLAYLISTS\np\nAll>");
}
BOOST_AUTO_TEST_CASE(test_pl_add_remove_and_get_track)
{
  save_t saver;
  pls_t db;
  db.add("All", pl_t{});
  pls_iter_t pl = db.begin();
  pl_iter_t tr = db.get("All").begin();
  std::ostringstream out("");
  std::istringstream in("./dirko.nikita/F0/tr1.wav t1 p p t1 p p t1 p");
  std::ostringstream dump("");
  add(in, dump, db, tr, pl, saver);
  pl_add(in, dump, db, tr, pl, saver);
  pl_add_track(in, dump, db, tr, pl, saver);
  pl_get(in, out, db, tr, pl, saver);
  BOOST_CHECK(out.str() == "<p\nt1>");
  out.str("");
  pl_remove_track(in, dump, db, tr, pl, saver);
  pl_get(in, out, db, tr, pl, saver);
  BOOST_CHECK(out.str() == "<p>");
}
BOOST_AUTO_TEST_CASE(test_pl_select)
{
  save_t saver;
  pls_t db;
  db.add("All", pl_t{});
  pls_iter_t pl = db.begin();
  pl_iter_t tr = db.get("All").begin();
  std::ostringstream out("");
  std::istringstream in("./dirko.nikita/F0/tr1.wav t1 ./dirko.nikita/F0/tr2.wav t2 p p t1 p");
  std::ostringstream dump("");
  add(in, dump, db, tr, pl, saver);
  add(in, dump, db, tr, pl, saver);
  pl_add(in, dump, db, tr, pl, saver);
  pl_add_track(in, dump, db, tr, pl, saver);
  pl_select(in, dump, db, tr, pl, saver);
  list(in, out, db, tr, pl, saver);
  BOOST_CHECK(out.str() == "<p\nt1>");
}
BOOST_AUTO_TEST_CASE(test_pl_merge)
{
  save_t saver;
  pls_t db;
  db.add("All", pl_t{});
  pls_iter_t pl = db.begin();
  pl_iter_t tr = db.get("All").begin();
  std::ostringstream out("");
  std::istringstream in("./dirko.nikita/F0/tr1.wav t1 ./dirko.nikita/F0/tr2.wav t2 ./dirko.nikita/F0/tr3.wav t3 p1 p2 "
                        "p1 t1 p1 t2 p2 t2 p2 t3 p1 p2 p3 p3");
  std::ostringstream dump("");
  add(in, dump, db, tr, pl, saver);
  add(in, dump, db, tr, pl, saver);
  add(in, dump, db, tr, pl, saver);
  pl_add(in, dump, db, tr, pl, saver);
  pl_add(in, dump, db, tr, pl, saver);
  pl_add_track(in, dump, db, tr, pl, saver);
  pl_add_track(in, dump, db, tr, pl, saver);
  pl_add_track(in, dump, db, tr, pl, saver);
  pl_add_track(in, dump, db, tr, pl, saver);
  pl_merge(in, dump, db, tr, pl, saver);
  pl_get(in, out, db, tr, pl, saver);
  BOOST_CHECK(db.has("p3"));
  BOOST_CHECK(out.str() == "<p3\nt2\nt3\nt1>");
}
BOOST_AUTO_TEST_CASE(test_pl_diff)
{
  save_t saver;
  pls_t db;
  db.add("All", pl_t{});
  pls_iter_t pl = db.begin();
  pl_iter_t tr = db.get("All").begin();
  std::ostringstream out("");
  std::istringstream in("./dirko.nikita/F0/tr1.wav t1 ./dirko.nikita/F0/tr2.wav t2 ./dirko.nikita/F0/tr3.wav t3 p1 p2 "
                        "p1 t1 p1 t2 p2 t2 p2 t3 p1 p2 p3 p3");
  std::ostringstream dump("");
  add(in, dump, db, tr, pl, saver);
  add(in, dump, db, tr, pl, saver);
  add(in, dump, db, tr, pl, saver);
  pl_add(in, dump, db, tr, pl, saver);
  pl_add(in, dump, db, tr, pl, saver);
  pl_add_track(in, dump, db, tr, pl, saver);
  pl_add_track(in, dump, db, tr, pl, saver);
  pl_add_track(in, dump, db, tr, pl, saver);
  pl_add_track(in, dump, db, tr, pl, saver);
  pl_diff(in, dump, db, tr, pl, saver);
  pl_get(in, out, db, tr, pl, saver);
  BOOST_CHECK(db.has("p3"));
  BOOST_CHECK(out.str() == "<p3\nt3\nt1>");
}
BOOST_AUTO_TEST_SUITE_END()
