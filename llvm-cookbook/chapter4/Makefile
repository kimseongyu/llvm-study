CC = clang
PASS1 = func_block_count
PASS2 = opcode_count
PASS3 = everything_must_alias
PASS1_NAME = func-block-count
PASS2_NAME = opcode-count
PASS3_NAME = everything-must-alias

build :
	clang-format -style=google -i $(PASS1).cpp
	$(CC)++ -fPIC -shared $(PASS1).cpp -o $(PASS1).dylib `llvm-config --cxxflags --ldflags --system-libs --libs core` -O3
	clang-format -style=google -i $(PASS2).cpp
	$(CC)++ -fPIC -shared $(PASS2).cpp -o $(PASS2).dylib `llvm-config --cxxflags --ldflags --system-libs --libs core` -O3
	clang-format -style=google -i $(PASS3).cpp
	$(CC)++ -fPIC -shared $(PASS3).cpp -o $(PASS3).dylib `llvm-config --cxxflags --ldflags --system-libs --libs core` -O3

run1 : 
	$(CC) -S -O0 -emit-llvm $(PASS1)_test.c
	sed -i '' 's/optnone//g' $(PASS1)_test.ll
	opt -load-pass-plugin $(PASS1).dylib -passes=$(PASS1_NAME) $(PASS1)_test.ll -disable-output

run2 : 
	$(CC) -S -O0 -emit-llvm $(PASS2)_test.c -o $(PASS2)_test.bc
	opt -load-pass-plugin $(PASS2).dylib -passes=$(PASS2_NAME) $(PASS2)_test.bc -disable-output

run3 :
	$(CC) -S -O0 -emit-llvm $(PASS2)_test.c -o $(PASS2)_test.ll
	sed -i '' 's/optnone//g' $(PASS2)_test.ll
	opt -load-pass-plugin $(PASS3).dylib -passes="$(PASS3_NAME),aa-eval" $(PASS2)_test.ll -disable-output

clean :
	rm $(TARGET)