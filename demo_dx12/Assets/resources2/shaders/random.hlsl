#aiva init_meta_block
	entry_for_comp = main
#aiva shut_meta_block

#aiva init_code_block
	[numthreads(1, 1, 1)]
	void main(uint3 DTid : SV_DispatchThreadID)
	{
		
	}
#aiva shut_code_block
