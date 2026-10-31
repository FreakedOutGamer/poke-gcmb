# Pokémon Colosseum and Pokémon Channel multiboot programs

This is a decompilation of the GBA programs that Pokémon Colosseum, the
Pokémon Colosseum Bonus Disc and Pokémon Channel send over the link cable to
a Game Boy Advance running Ruby, Sapphire, Emerald, FireRed or LeafGreen. Each program is its own project
directory:

* [**client**](client/README.md): the Bonus Disc's Jirachi distribution, both revisions
  * client_2003_1112.mb `sha1: 446b9a3d3abe45236ef4a378e6469496ae85aa5c`
  * client.mb `sha1: 39287eeba84ca1c4c4e4eeed36fcc36390364b99`
* [**colbtl**](colbtl/README.md): Colosseum Battle, for battles against the GameCube, in seven languages
  * colbtl.mb `sha1: 106b51c41138fd8103c7b5b300741f050e6299eb`
  * colbtl_e.mb `sha1: cc760666e6a83ba5a85d0146ca716c88e9a3d6ac`
  * colbtl_f.mb `sha1: 7ab16dd3442eae66d6006afd3cc583de9c3cf51c`
  * colbtl_g.mb `sha1: 88908d4d208f808b3cd2c31d1ae3b33a5c77be7d`
  * colbtl_i.mb `sha1: 647bf463a85107488487a73e0892a7a86b09958e`
  * colbtl_jp.mb `sha1: 002d916e92cbc69246b56b0c7ad54c9f9bfaa8a0`
  * colbtl_s.mb `sha1: 3c5421eed214b58eab45caaca142aae4717c9f69`
* [**coupon**](coupon/README.md): Colosseum's Poké Coupon link
  * coupon.mb `sha1: 9f170f72a7b06ee1e533f1e3c6812e810f772784`
* [**pokechan**](pokechan/README.md): Pokémon Channel's Jirachi distribution
  * pokechan.mb `sha1: 32429c491ae83f84b2a71fe5523fccd215aaab59`
* [**pkjb_test**](pkjb_test/README.md): a test program left on the Colosseum discs
  * pkjb_test.mb `sha1: 215e873588838def7c52e118682a1b33c6d6eb6a`
* [**sample0519**](sample0519/README.md): a Japanese sample build of the Jirachi distribution
  * sample0519.mb `sha1: fe72fb983b623aa560df84e9d269e7a21db937e4`

Every image builds byte-identical to the original.

To set up the repository, see [INSTALL.md](INSTALL.md).


## See also

The pret projects these build on:
* [**Pokémon Colosseum's multiboot image**](https://github.com/pret/colosseum-mb), colbtl's sibling
* [**Berry Fix**](https://github.com/pret/berry-fix)
* [**Pokémon Ruby and Sapphire**](https://github.com/pret/pokeruby)
* [**Pokémon Emerald**](https://github.com/pret/pokeemerald)
* [**Pokémon FireRed and LeafGreen**](https://github.com/pret/pokefirered)

For pret's other projects, see [pret.github.io](https://pret.github.io/).
