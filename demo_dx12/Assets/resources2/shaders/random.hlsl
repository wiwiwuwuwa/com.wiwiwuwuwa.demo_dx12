#aiva init_meta_block
entry_for_comp = main
#aiva shut_meta_block

#aiva init_code_block
// init structures ----------------------------------------
struct constant_buffer_t
{
	float4 m_test0;
	float4 m_test1;
	float4 m_test2;
	float4 m_test3;
};
// shut structures ----------------------------------------

// init variables ----------------------------------------
ConstantBuffer<constant_buffer_t> m_constant_buffer : register(b0);
// shut variables ----------------------------------------

// init functions ----------------------------------------
[numthreads(1, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
		
}
// shut functions ----------------------------------------
#aiva shut_code_block
