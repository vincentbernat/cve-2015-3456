SUBDIRS = mock exploit patch
.DEFAULT_GOAL = all

%:
	@for dir in $(SUBDIRS); do $(MAKE) -C $$dir $@ ; done
