SUBDIRS := test

.PHONY: all $(SUBDIRS)

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

.PHONY: FORCE
%: FORCE
	@for dir in $(SUBDIRS); do \
	    $(MAKE) -C $$dir $@ || exit 1; \
	done
