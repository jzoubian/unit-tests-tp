/*****************************************************
             PROJECT  : MALT
             VERSION  : 1.2.2
             DATE     : 10/2020
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <gtest/gtest.h>
#include <stacks/BacktraceStack.hpp>
#include <common/SimpleAllocator.hpp>
#include <common/Options.hpp>

/***************** USING NAMESPACE ******************/
using namespace MALT;

/*******************  FUNCTION  *********************/
//mock the backtrace libc function
extern "C" {
size_t gblGenStackSize = 0;
int backtrace(void ** buffer, int size) {
	EXPECT_GE(size, gblGenStackSize);
	for (size_t i = 0 ; i < gblGenStackSize ; i++)
		buffer[i] = (void*)(0x2+i);
	return gblGenStackSize;
}
}

/*******************  FUNCTION  *********************/
TEST(Stack,constructorAndLoadCurrentStack_len_2)
{
	gblGenStackSize = 2;

	BacktraceStack stack;
	stack.loadCurrentStack();
	
	std::stringstream buffer;
	buffer << stack;
	
	EXPECT_EQ("0x1 0x2 ",buffer.str());
}

/*******************  FUNCTION  *********************/
TEST(Stack,constructorAndLoadCurrentStack_len_4)
{
	gblGenStackSize = 4;

	BacktraceStack stack;
	stack.loadCurrentStack();
	
	std::stringstream buffer;
	buffer << stack;
	
	EXPECT_EQ("0x1 0x2 0x3 0x4 ",buffer.str());
}

/*******************  FUNCTION  *********************/
TEST(Stack,constructorAndLoadCurrentStack_len_8)
{
	gblGenStackSize = 8;

	BacktraceStack stack;
	stack.loadCurrentStack();
	
	std::stringstream buffer;
	buffer << stack;
	
	EXPECT_EQ("0x1 0x2 0x3 0x4 0x5 0x6 0x7 0x8 ",buffer.str());
}

/*******************  FUNCTION  *********************/
int main(int argc, char ** argv)
{
	//init internal allocator
	gblInternaAlloc = new SimpleAllocator(true);
	gblOptions = new Options;
	
	// This allows the user to override the flag on the command line.
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
