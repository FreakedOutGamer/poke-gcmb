# Pokémon Colosseum (Colosseum Battle)

This is a decompilation of `colbtl.bin`, the multiboot program Pokémon
Colosseum sends to Ruby and Sapphire for battles against the GameCube, in
all seven known language versions. It shares a code base with pret's
[colosseum-mb](https://github.com/pret/colosseum-mb).

It builds the following images:

* **colbtl.mb** `sha1: 106b51c41138fd8103c7b5b300741f050e6299eb`
* **colbtl_e.mb** `sha1: cc760666e6a83ba5a85d0146ca716c88e9a3d6ac`
* **colbtl_f.mb** `sha1: 7ab16dd3442eae66d6006afd3cc583de9c3cf51c`
* **colbtl_g.mb** `sha1: 88908d4d208f808b3cd2c31d1ae3b33a5c77be7d`
* **colbtl_i.mb** `sha1: 647bf463a85107488487a73e0892a7a86b09958e`
* **colbtl_jp.mb** `sha1: 002d916e92cbc69246b56b0c7ad54c9f9bfaa8a0`
* **colbtl_s.mb** `sha1: 3c5421eed214b58eab45caaca142aae4717c9f69`

The European disc (GC6P01) carries six of them as `files/colbtl.bin`,
`colbtl_e.bin`, `colbtl_f.bin`, `colbtl_g.bin`, `colbtl_i.bin` and
`colbtl_s.bin`; the Japanese disc (GC6J01) carries `colbtl_jp` as
`files/colbtl.bin`, as do the expansion discs' `pokecolo_ex.tgc`. The US
disc (GC6E01) has none.

To set up the repository, see [INSTALL.md](../INSTALL.md).

