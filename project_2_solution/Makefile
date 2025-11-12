CXXSOURCES = $(shell find . -not -path './examples/*' -name '*.cpp')
CSOURCES = $(shell find . -not -path './y.tab.c' -and -not -path './lex.yy.c' -name '*.c')
CSOURCES += y.tab.c
HEADERS = $(shell find . -not -path './y.tab.h' -name '*.h')
HEADERS += y.tab.h
OBJ += $(CXXSOURCES:%.cpp=%.o)
OBJ += $(CSOURCES:%.c=%.o)

TESTS = $(wildcard tests/*.mjava)
OUTPUTS := $(foreach test,$(TESTS),outputs/$(test:tests/%.mjava=%).out)
OUTPUT_DIFFS := $(foreach test,$(TESTS),output_diffs/$(test:tests/%.mjava=%).diff)
LINE_OUTPUTS := $(foreach test,$(TESTS),line_outputs/$(test:tests/%.mjava=%).out)
LINE_DIFFS := $(foreach test,$(TESTS),line_diffs/$(test:tests/%.mjava=%).diff)
ASTS := $(foreach test,$(TESTS),asts/$(test:tests/%.mjava=%).png)
AST_DIFFS := $(foreach test,$(TESTS),ast_diffs/$(test:tests/%.mjava=%).png)

ERRORS = $(wildcard errors/*.mjava)
ERROR_OUTPUTS := $(foreach error,$(ERRORS),error_outputs/$(error:errors/%.mjava=%).out)
ERROR_DIFFS := $(foreach error,$(ERRORS),error_diffs/$(error:errors/%.mjava=%).diff)

CXX = clang++-12 -stdlib=libc++
CC = clang-12
CFLAGS = -g -I .
LLVMCXXFLAGS = `llvm-config-12 --cxxflags`
LLVMCFLAGS = `llvm-config-12 --cflags`
LLVMFLAGS = `llvm-config-12 --cxxflags --ldflags --system-libs --libs all`

FLEX = flex
YACC = yacc

all: build test asts
build: parser
test: $(OUTPUTS) $(OUTPUT_DIFFS) $(LINE_OUTPUTS) $(LINE_DIFFS) $(ERROR_OUTPUTS) $(ERROR_DIFFS)
asts: $(ASTS) $(AST_DIFFS)

parser: ${OBJ}
	${CXX} ${CFLAGS} ${LLVMFLAGS} -o parser ${OBJ} -lfl

lex.yy.c: lex.l
	$(FLEX) lex.l

# y.tab.h: grammar.y lex.yy.c
# 	$(YACC) -d -v grammar.y

# y.tab.c: grammar.y lex.yy.c
# 	$(YACC) -d -v grammar.y

%.o: %.cpp ${HEADERS}
	${CXX} ${CFLAGS} ${LLVMCXXFLAGS} -c $< -o $@

%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} ${LLVMCFLAGS} -c $< -o $@


define test_rules
outputs/$(1:tests/%.mjava=%).out: parser $(1)
	@echo "./parser -v $(1) > $$@"
	-@./parser -v $(1) > $$@
endef
$(foreach test,$(TESTS),$(eval $(call test_rules,$(test))))

define outputdiff_rules
output_diffs/$(1:tests/%.mjava=%).diff: outputs/$(1:tests/%.mjava=%).out
	@echo "diff -d $$< outputs_solution/$(1:tests/%.mjava=%).out > $$@"
	-@diff -d $$< outputs_solution/$(1:tests/%.mjava=%).out > $$@
endef
$(foreach test,$(TESTS),$(eval $(call outputdiff_rules,$(test))))

define linetest_rules
line_outputs/$(1:tests/%.mjava=%).out: parser $(1)
	@echo "./parser -v -l $(1) > $$@"
	-@./parser -v -l $(1) > $$@
endef
$(foreach test,$(TESTS),$(eval $(call linetest_rules,$(test))))

define linediff_rules
line_diffs/$(1:tests/%.mjava=%).diff: line_outputs/$(1:tests/%.mjava=%).out
	@echo "diff -d $$< line_outputs_solution/$(1:tests/%.mjava=%).out > $$@"
	-@diff -d $$< line_outputs_solution/$(1:tests/%.mjava=%).out > $$@
endef
$(foreach test,$(TESTS),$(eval $(call linediff_rules,$(test))))

define error_rules
error_outputs/$(1:errors/%.mjava=%).out: parser $(1)
	@echo "./parser -w $(1) > $$@"
	-@./parser -w $(1) > $$@
endef
$(foreach error,$(ERRORS),$(eval $(call error_rules,$(error))))

define error_diff_rules
error_diffs/$(1:errors/%.mjava=%).diff: error_outputs/$(1:errors/%.mjava=%).out
	@echo "diff -d $$< error_outputs_solution/$(1:errors/%.mjava=%).out > $$@"
	-@diff -d $$< error_outputs_solution/$(1:errors/%.mjava=%).out > $$@
endef
$(foreach error,$(ERRORS),$(eval $(call error_diff_rules,$(error))))

define ast_rules
asts/$(1:tests/%.mjava=%).png: parser $(1)
	@echo "./parser -v -p asts/$(1:tests/%.mjava=%).gv $(1) > $$@"
	-@./parser -v -p asts/$(1:tests/%.mjava=%).gv $(1) > $$@
	@echo "gv -Tpng asts/$(1:tests/%.mjava=%).gv > asts/$(1:tests/%.mjava=%).png"
	-@dot -Tpng asts/$(1:tests/%.mjava=%).gv > asts/$(1:tests/%.mjava=%).png
endef
$(foreach test,$(TESTS),$(eval $(call ast_rules,$(test))))

define ast_diff_rules
ast_diffs/$(1:tests/%.mjava=%).png: asts/$(1:tests/%.mjava=%).gv
	@echo "awk -f dotdiff.awk $$< asts_solution/$(1:tests/%.mjava=%).gv > ast_diffs/$(1:tests/%.mjava=%).gv"
	-@awk -f dotdiff.awk $$< asts_solution/$(1:tests/%.mjava=%).gv > ast_diffs/$(1:tests/%.mjava=%).gv
	@echo "dot -Tpng ast_diffs/$(1:tests/%.mjava=%).gv > $$@"
	-@dot -Tpng ast_diffs/$(1:tests/%.mjava=%).gv > $$@
endef
$(foreach test,$(TESTS),$(eval $(call ast_diff_rules,$(test))))

clean:
# 	rm -f parser y.tab.* lex.yy.c ${OBJ} outputs/*.out output_diffs/*.diff line_outputs/*.out line_diffs/*.diff error_outputs/*.out error_diffs/*.diff asts/*.png ast_diffs/*.png asts/*.gv ast_diffs/*.gv
	rm -f parser lex.yy.c $(filter-out y.tab.o, $(OBJ)) outputs/*.out output_diffs/*.diff line_outputs/*.out line_diffs/*.diff error_outputs/*.out error_diffs/*.diff asts/*.png ast_diffs/*.png asts/*.gv ast_diffs/*.gv