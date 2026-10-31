# Builds the tools every project needs, from source in tools/, and checks
# every project.
#
#   make tools          agbcc, gbagfx and preproc (once, before any project)
#   make compare        build and check every finished project
#   make clean-tools    remove the built tools
#
# agbcc comes from the tools/agbcc-src submodule (pret/agbcc); after a fresh
# clone, run `git submodule update --init` first. gbagfx needs libpng.

AGBCC := tools/agbcc/bin/agbcc
GBAGFX := tools/gbagfx/gbagfx
PREPROC := tools/preproc/preproc

PROJECTS := pkjb_test pokechan client sample0519 coupon colbtl

.PHONY: tools compare clean-tools $(PROJECTS)

tools: $(AGBCC) $(GBAGFX) $(PREPROC)

# build.sh builds agbcc, old_agbcc, agbcc_arm, libgcc and libc inside the
# submodule; install.sh copies them, with the headers, into tools/agbcc.
$(AGBCC):
	@test -f tools/agbcc-src/build.sh || { echo "tools/agbcc-src is empty: run git submodule update --init"; exit 1; }
	cd tools/agbcc-src && ./build.sh && ./install.sh $(CURDIR)

$(GBAGFX):
	$(MAKE) -C tools/gbagfx

$(PREPROC):
	$(MAKE) -C tools/preproc

compare: $(PROJECTS)

$(PROJECTS): tools
	$(MAKE) -C $@ compare

clean-tools:
	rm -rf tools/agbcc
	$(MAKE) -C tools/gbagfx clean
	$(MAKE) -C tools/preproc clean
