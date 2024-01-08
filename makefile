
PROG := bin\vic.exe
SRCS := $(wildcard *.cpp) # wildcard関数を用いてファイル内の.cppを全て取得する(配列)
OBJS := $(SRCS:%.cpp=%.o) # %マクロを用いて置換 SRC配列を元に"<ファイル名>.o"の配列を作る。 中間オブジェクトファイル用。
DEPS := $(SRCS:%.cpp=%.d) # %マクロを用いて置換 SRC配列を元に"<ファイル名>.d"の配列を作る。 依存ファイル用。

# 各種設定を変数として定義
CC := clang
CCFLAGS := -std=c++2b
INCLUDEPATH :=
LIBPATH :=
LIBS :=

# これが主レシピ
all: $(DEPENDS) $(PROG)

# リンク
$(PROG): $(OBJS)
	$(CC) $(CCFLAGS) -o $@ $^ $(LIBPATH) $(LIBS)

# コンパイル
.cpp.o:
	$(CC) $(CCFLAGS) $(INCLUDEPATH) -MMD -MP -MF $(<:%.cpp=%.d) -c $< -o $(<:%.cpp=%.o)

# "make clean"でターゲットと中間ファイルを消去できるようにする
.PHONY: clean
clean:
	del $(PROG) $(OBJS) $(DEPS)
.PHONY: test
test: all
	bin/vic.exe test.vi
.PHONY: testclean
testclean:
	del test.obj test.asm test.exe


-include $(DEPS) # include "ファイル名" でそのファイルの内容をここにコピペしたのと同じ効果を得られる
