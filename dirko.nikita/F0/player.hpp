#ifndef PLAYER_TYPES_HPP
#define PLAYER_TYPES_HPP
#include <string>
#include "decoder.hpp"
#include "robin.hpp"

#define USE_FAKE_SDL
namespace dirko
{
  struct Track
  {
    WavFile track_;
    float duration_;
    Track() = default;
    Track(WavFile file);
  };

  using pl_t = RobinTable< std::string, Track >;
  using pl_iter_t = RTIter< std::string, Track >;
  using pls_t = RobinTable< std::string, pl_t >;
  using pls_iter_t = RTIter< std::string, pl_t >;
  using save_t = RobinTable< std::string, std::string >;

  void PlayWav(dirko::WavFile &wav, float dur);
  void save(const save_t &saver, const pls_t &db);
  void load(save_t &saver, std::ostream &out, pls_t &playlists);
}
#endif
