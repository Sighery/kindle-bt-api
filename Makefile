.PHONY: format
format:
	@clang-format -i -style=file -- src/**.c src/include/**.h
