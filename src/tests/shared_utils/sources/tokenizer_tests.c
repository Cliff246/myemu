#include "tests_manager.h"
#include "tokenizer.h"




TEST(tokenizer_brackets1)
{
	p_tok_t token = split_str_into_tokens("the \'cat\' jumps over the (dhuge(o)g)", " ");
	p_tok_t dest;	
	printf("tests tokenizer_brackets\n");
	int flag = pair_toks_delims(token,&dest,"\'\'()");

	
	return 0;
	

}
TEST(tokenizer_brackets2)
{
	const char *str = "hello(world)(you)(are(super))annoying(please(give)me(a)break)";
	p_tok_t token = split_str_into_tokens((char *)str, " ");
	p_tok_t dest;	
	printf("tests tokenizer_brackets\n");
	int flag = pair_toks_delims(token,&dest,"\'\'()");
	printf("%s problem\n", str);
	
	return 0;
	

}



TEST_COLLECTOR(tokenizer)
{
	REGISTER_TEST(tokenizer_brackets1, NOSKIP);
	REGISTER_TEST(tokenizer_brackets2, NOSKIP);
}
