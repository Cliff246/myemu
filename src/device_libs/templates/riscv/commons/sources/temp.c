#include "temp.h"
#include "commons.h"
#include "tokenizer.h"

struct riscv_temp_inst_32 
{
	char *mnemonic;
	char fmt[2];
	
};

static struct riscv_temp_inst_32 riscv_insts[] = 
{
	{.mnemonic = "", .fmt[0] = 'r', .fmt[1] = ' '}		

};


uint32_t TMP_RISCV_decode_riscv_format_R(char *opcode, char *rd, char *funct3, char *rs1, char *rs2, char *funct7)
{
	const int opcode_len = strlen(opcode);
	const int rd_len = strlen(rd);
	const int funct3_len = strlen(funct3);
	const int rs1_len = strlen(rs1); 
	
	const int rs2_len = strlen(rs2); 
	const int funct7_len = strlen(funct7);
	const int sum = opcode_len + rd_len + funct3_len + rs1_len + rs2_len + funct7_len;
	if(sum != 32)
	{
		fprintf(stderr,"sum must mach 32 bits: was %d\n", sum);
		exit(1);
	}
	if(
			opcode_len == 7 && 
			rd_len == 5 &&
			funct3_len == 3 &&
			rs1_len == 5 &&
			rs2_len == 5 &&
			funct7_len == 7)
	{
		uint32_t final_binary = 0;
		const uint64_t opcode_bin = convert_str_to_binary(opcode);	
		const uint64_t rd_bin = convert_str_to_binary(rd);
		const uint64_t funct3_bin = convert_str_to_binary(funct3);
		const uint64_t rs1_bin = convert_str_to_binary(rs1);
		const uint64_t rs2_bin = convert_str_to_binary(rs2);
		const uint64_t funct7_bin = convert_str_to_binary(funct7);
		final_binary |= (funct7_bin & 0x7f)	<< 25;
		final_binary |= (rs2_bin & 0x1f) << 20;
		final_binary |= (rs1_bin & 0x1f) << 15;

		final_binary |= (funct3_bin & 0x07) << 12;
		final_binary |= (rd_bin & 0x1f) << 7;
		final_binary |= opcode_bin & 0x7f;
		return final_binary;	
	}
	else
	{
		fprintf(stderr, "sizes of binaries did not meet expected sizes\n");
		exit(1);
	}
}



uint32_t TMP_RISCV_decode_riscv_format_I(char *opcode, char *rd, char *funct3, char *rs1, char *imm)
{
	const int opcode_len = strlen(opcode);
	const int rd_len = strlen(rd);
	const int funct3_len = strlen(funct3);
	const int rs1_len = strlen(rs1); 
	const int imm_len = strlen(imm);	
	const int sum = imm_len + rs1_len + funct3_len + rd_len + opcode_len;
	if(sum != 32)
	{
		fprintf(stderr,"sum must mach 32 bits: was %d\n", sum);
		exit(1);
	}
	if(
			opcode_len == 7 && 
			rd_len == 5 &&
			funct3_len == 3 &&
			rs1_len == 5 &&
			imm_len == 12 
	)
	{
		uint32_t final_binary = 0;
		const uint64_t opcode_bin = convert_str_to_binary(opcode);	
		const uint64_t rd_bin = convert_str_to_binary(rd);
		const uint64_t funct3_bin = convert_str_to_binary(funct3);
		const uint64_t rs1_bin = convert_str_to_binary(rs1);
		const uint64_t imm_bin = convert_str_to_binary(imm);
		final_binary |= (imm_bin & 0xfff )<< 20 ;

		final_binary |= (rs1_bin & 0x1f) << 15;

		final_binary |= (funct3_bin & 0x07) << 12;
		final_binary |= (rd_bin & 0x1f) << 7;
		final_binary |= opcode_bin & 0x7f;
		return final_binary;	
	}
	else
	{
		fprintf(stderr, "sizes of binaries did not meet expected sizes\n");
		exit(1);
	}
}
uint32_t TMP_RISCV_decode_riscv_format_S(char *opcode, char *imm1, char *funct3, char *rs1, char *rs2,  char *imm2)
{
	const int opcode_len = strlen(opcode);
	const int imm1_len = strlen(imm1);
	const int funct3_len = strlen(funct3);
	const int rs1_len = strlen(rs1); 
	
	const int rs2_len = strlen(rs2); 
	const int imm2_len = strlen(imm2);
	const int sum = imm1_len + rs1_len + funct3_len + opcode_len + imm2_len;
	if(sum != 32)
	{
		fprintf(stderr,"sum must mach 32 bits: was %d\n", sum);
		exit(1);
	}
	if(
			opcode_len == 7 && 
			imm1_len == 5 &&
			funct3_len == 3 &&
			rs1_len == 5 &&
			imm2_len == 12 
	)
	{
		uint32_t final_binary = 0;
		const uint64_t opcode_bin = convert_str_to_binary(opcode);	
		const uint64_t imm1_bin = convert_str_to_binary(imm1);
		const uint64_t funct3_bin = convert_str_to_binary(funct3);
		const uint64_t rs1_bin = convert_str_to_binary(rs1);
		const uint64_t imm2_bin = convert_str_to_binary(imm2);
		final_binary |= ((imm2_bin >> 5)  & 0x7f)<< 25;

		final_binary |= (rs1_bin & 0x1f) << 15;

		final_binary |= (funct3_bin & 0x07) << 12;
		final_binary |= (imm1_bin & 0x1f) << 7;
		final_binary |= opcode_bin & 0x7f;
		return final_binary;	
	}
	else
	{
		fprintf(stderr, "sizes of binaries did not meet expected sizes\n");
		exit(1);
	}

}


uint32_t TMP_RISCV_decode_riscv_format_SB(char *opcode, char *imm1, char *funct3, char *rs1, char *rs2,  char *imm2)
{
	const int opcode_len = strlen(opcode);
	const int imm1_len = strlen(imm1);
	const int funct3_len = strlen(funct3);
	const int rs1_len = strlen(rs1); 
	
	const int rs2_len = strlen(rs2); 
	const int imm2_len = strlen(imm2);
	const int sum = imm1_len + rs1_len + funct3_len + opcode_len + imm2_len;
	if(sum != 32)
	{
		fprintf(stderr,"sum must mach 32 bits: was %d\n", sum);
		exit(1);
	}
	if(
			opcode_len == 7 && 
			imm1_len == 5 && 
			funct3_len == 3 &&
			rs1_len == 5 &&
			imm2_len == 12 
	)
	{
		uint32_t final_binary = 0;
		const uint64_t opcode_bin = convert_str_to_binary(opcode);	
		const uint64_t imm1_bin = convert_str_to_binary(imm1);
		const uint64_t funct3_bin = convert_str_to_binary(funct3);
		const uint64_t rs1_bin = convert_str_to_binary(rs1);
		const uint64_t imm2_bin = convert_str_to_binary(imm2);
		final_binary |= ((imm2_bin >> 5)  & 0x7f)<< 25;

		final_binary |= (rs1_bin & 0x1f) << 15;

		final_binary |= (funct3_bin & 0x07) << 12;
		final_binary |= (imm1_bin & 0x1f) << 7;
		final_binary |= opcode_bin & 0x7f;
		return final_binary;	
	}
	else
	{
		fprintf(stderr, "sizes of binaries did not meet expected sizes\n");
		exit(1);
	}
}


uint32_t TMP_RISCV_decode_riscv_format_U(char *opcode, char *rd,char *imm)
{
	const int opcode_len = strlen(opcode);
	const int rd_len = strlen(rd);
	const int imm_len = strlen(imm);	
	const int sum = opcode_len + rd_len + imm_len; 
	if(sum != 32)
	{
		fprintf(stderr,"sum must mach 32 bits: was %d\n", sum);
		exit(1);
	}
	if(
			opcode_len == 7 && 
			rd_len == 5 &&
			imm_len == 20 
	)
	{
		uint32_t final_binary = 0;
		const uint64_t opcode_bin = convert_str_to_binary(opcode);	
		const uint64_t imm_bin = convert_str_to_binary(imm);
		const uint64_t rd_bin = convert_str_to_binary(rd);
		final_binary |= (imm_bin & 0xFFFFF) << 12;
		final_binary |= (rd_bin & 0x1f) << 7;
		final_binary |= opcode_bin & 0x7f;
		return final_binary;	
	}
	else
	{
		fprintf(stderr, "sizes of binaries did not meet expected sizes\n");
		exit(1);
	}
}

uint32_t TMP_RISCV_decode_riscv_format_UJ(char *opcode, char *rd,char *imm)
{
	const int opcode_len = strlen(opcode);
	const int rd_len = strlen(rd);
	const int imm_len = strlen(imm);	
	const int sum = opcode_len + rd_len + imm_len; 
	if(sum != 32)
	{
		fprintf(stderr,"sum must mach 32 bits: was %d\n", sum);
		exit(1);
	}
	if(
			opcode_len == 7 && 
			rd_len == 5 &&
			imm_len == 20 
	)
	{
		uint32_t final_binary = 0;
		const uint64_t opcode_bin = convert_str_to_binary(opcode);	
		const uint64_t imm_bin = convert_str_to_binary(imm);
		const uint64_t rd_bin = convert_str_to_binary(rd);
		final_binary |= (imm_bin & 0xFFFFF) << 12;
		final_binary |= (rd_bin & 0x1f) << 7;
		final_binary |= opcode_bin & 0x7f;
		return final_binary;	
	}
	else
	{
		fprintf(stderr, "sizes of binaries did not meet expected sizes\n");
		exit(1);
	}
}


uint32_t TMP_RISCV_convert_string_to_binary(const char *assembly)
{
	
	p_tok_t tokens = split_str_into_tokens((char *)assembly, ", ()");
	int opcode = -1;	
	for(size_t i = 0; i < ARY_LENGTH(riscv_insts); ++i)
	{
		if(cmpstrings(riscv_insts[i].mnemonic,tokens->p_sz_toks[0]) == true)
		{
			opcode = i;
			break;
		}
	}
	char binary[32];


}
