#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "pipeline.h"
#include "machine.h"
#include "inst_process.h"

#define reg(x) reg_memory[x * 4].mem_data
#define mem(x) data_memory[x].mem_data


void inst_fetch(char* inst)
{
	printf("inst  == %s", inst);
	if (pipeline_next_stage_null(IFID) == true)
	{
		char *OP;
		char tmp[128];
		strcpy(tmp, inst);
		OP = strtok(tmp, "\t");
		char* operands = OP + strlen(OP) + 1;
		if (strcmp(OP, "\n") == 0)
			return;
		if (strcmp(OP, "UNUSE") == 0)
			return;
		if (strcmp(operands, "") == 0)
			return;
		strcpy(IFID.op, OP);
		strcpy(IFID.inst, operands);
		printf("OP: %s operands: %s\n", OP, operands);
		PC += 4;
	}
	/*else
	{
		printf("IFID\n");
		printf("rs: $%d rt: $%d rd: $%d imm: %d temp: %d\n", IFID.rs, IFID.rt, IFID.rd, IFID.imm, IFID.temp);
		printf("op: %s inst: %s\n", IFID.op, IFID.inst);
		printf("PC %d\n", PC);
	}*/
	return;
}
void inst_decode(void)
{
	long r_d, r_s, r_t, C, shamt;
	char reg_dest[128];
	if (pipeline_next_stage_null(IDEX) == true)
	{
		pass_register_content(&IFID, &IDEX);
		if (strcmp(IFID.op, "ADD") == 0)
		{
			sscanf(IFID.inst, "$%ld, $%ld, $%ld", &r_d, &r_s, &r_t);
			IDEX.rd = r_d;
			IDEX.rs = r_s;
			IDEX.rt = r_t;
		}
		if (strcmp(IFID.op, "SUB") == 0)
		{
			sscanf(IFID.inst, "$%ld, $%ld, $%ld", &r_d, &r_s, &r_t);
			IDEX.rd = r_d;
			IDEX.rs = r_s;
			IDEX.rt = r_t;
		}
		if (strcmp(IFID.op, "ADDI") == 0)
		{
			sscanf(IFID.inst, "$%ld, $%ld, %ld", &r_d, &r_s, &r_t);
			IDEX.rd = r_d;
			IDEX.rs = r_s;
			IDEX.imm = r_t;
		}
		if (strcmp(IFID.op, "SUBI") == 0)
		{
			sscanf(IFID.inst, "$%ld, $%ld, %ld", &r_d, &r_s, &r_t);
			IDEX.rd = r_d;
			IDEX.rs = r_s;
			IDEX.imm = r_t;
		}
		if (strcmp(IFID.op, "OR") == 0)
		{
			sscanf(IFID.inst, "$%ld, $%ld, $%ld", &r_d, &r_s, &r_t);
			IDEX.rd = r_d;
			IDEX.rs = r_s;
			IDEX.rt = r_t;
		}
		if (strcmp(IFID.op, "AND") == 0)
		{
			sscanf(IFID.inst, "$%ld, $%ld, $%ld", &r_d, &r_s, &r_t);
			IDEX.rd = r_d;
			IDEX.rs = r_s;
			IDEX.rt = r_t;
		}
		if (strcmp(IFID.op, "SLL") == 0)
		{
			sscanf(IFID.inst, "$%ld, $%ld, %ld", &r_d, &r_s, &r_t);
			IDEX.rd = r_d;
			IDEX.rs = r_s;
			IDEX.imm = r_t;
		}
		if (strcmp(IFID.op, "SRL") == 0)
		{
			sscanf(IFID.inst, "$%ld, $%ld, %ld", &r_d, &r_s, &r_t);
			IDEX.rd = r_d;
			IDEX.rs = r_s;
			IDEX.imm = r_t;
		}
		if (strcmp(IFID.op, "LW") == 0)
		{
			sscanf(IFID.inst, "$%ld, %ld($%ld)", &r_d, &r_s, &r_t);
			IDEX.rd = r_d;
			IDEX.imm = r_s;
			IDEX.rt = r_t;
		}
		if (strcmp(IFID.op, "SW") == 0)
		{
			sscanf(IFID.inst, "$%ld, %ld($%ld)", &r_d, &r_s, &r_t);
			IDEX.rd = r_d;
			IDEX.imm = r_s;
			IDEX.rt = r_t;
		}
		if (strcmp(IFID.op, "LBU") == 0)
		{
			sscanf(IFID.inst, "$%ld, %ld($%ld)", &r_d, &r_s, &r_t);
			IDEX.rd = r_d;
			IDEX.imm = r_s;
			IDEX.rt = r_t;
		}
		if (strcmp(IFID.op, "SBU") == 0)
		{
			sscanf(IFID.inst, "$%ld, %ld($%ld)", &r_d, &r_s, &r_t);
			IDEX.rd = r_d;
			IDEX.imm = r_s;
			IDEX.rt = r_t;
		}
		if (strcmp(IFID.op, "SLT") == 0)
		{
			sscanf(IFID.inst, "$%ld, $%ld, $%ld", &r_d, &r_s, &r_t);
			IDEX.rd = r_d;
			IDEX.rs = r_s;
			IDEX.rt = r_t;
		}
		if (strcmp(IDEX.op, "J") == 0)
		{
			sscanf(IFID.inst, "%s", reg_dest);
			for (int i = 0; i < 2048; i += 4)
			{
				if (strcmp(inst_memory[i].data, "UNUSE") == 0)
					continue;
				if (strcmp(inst_memory[i].label, reg_dest) == 0)
				{
					PC = i;
					break;
				}
			}
			// control hazard stall again fetch
			clear_pipeline_register_content(&IFID);
			inst_fetch(inst_memory[PC].data);
			return;
		}
		if (strcmp(IFID.op, "BNE") == 0)
		{
			sscanf(IFID.inst, "$%ld, $%ld, %s", &r_d, &r_s, reg_dest);
			IDEX.rd = r_d;
			IDEX.rs = r_s;
			if (reg(r_d) != reg(r_s))
			{
				IDEX.temp = 1;
				for (int i = 0; i < 2048; i += 4)
				{
					if (strcmp(inst_memory[i].data, "UNUSE") == 0)
						continue;
					if (strcmp(inst_memory[i].label, reg_dest) == 0)
					{
						PC = i;
						break;
					}
				}
				// control hazard stall again fetch
				clear_pipeline_register_content(&IFID);
				inst_fetch(inst_memory[PC].data);
				return;
			}
			else
				IDEX.temp = 0;
		}
		if (strcmp(IFID.op, "BEQ") == 0)
		{
			sscanf(IFID.inst, "$%ld, $%ld, %s", &r_d, &r_s, reg_dest);
			IDEX.rd = r_d;
			IDEX.rs = r_s;
			if (reg(r_d) == reg(r_s)) 
			{
				IDEX.temp = 1;
				for (int i = 0; i < 2048; i += 4)
				{
					if (strcmp(inst_memory[i].data, "UNUSE") == 0)
						continue;
					if (strcmp(inst_memory[i].label, reg_dest) == 0)
					{
						PC = i;
						break;
					}
				}
				// control hazard stall again fetch
				clear_pipeline_register_content(&IFID);
				inst_fetch(inst_memory[PC].data);
				return;
			}
			else
				IDEX.temp = 0;
		}
		clear_pipeline_register_content(&IFID);
	}
	return;
}
void inst_execute(void)
{
	long r_d, r_s, r_t, C, shamt;
	char reg_dest[128];
	if (pipeline_next_stage_null(EXMEM) == true)
	{
		pass_register_content(&IDEX, &EXMEM);
		if (strcmp(IDEX.op, "ADD") == 0)
		{
			sscanf(IDEX.inst, "$%ld, $%ld, $%ld", &r_d, &r_s, &r_t);
			EXMEM.rd = r_d;
			EXMEM.temp = reg(r_s) + reg(r_t);
			//forwarding
			if (if_hazard() == true)
				reg(EXMEM.rd) = EXMEM.temp;
		}
		if (strcmp(IDEX.op, "SUB") == 0)
		{
			sscanf(IDEX.inst, "$%ld, $%ld, $%ld", &r_d, &r_s, &r_t);
			EXMEM.rd = r_d;
			EXMEM.temp = reg(r_s) - reg(r_t);
			//forwarding
			if (if_hazard() == true)
				reg(EXMEM.rd) = EXMEM.temp;
		}
		if (strcmp(IDEX.op, "ADDI") == 0)
		{
			sscanf(IDEX.inst, "$%ld, $%ld, %ld", &r_d, &r_s, &r_t);
			EXMEM.rd = r_d;
			EXMEM.temp = reg(r_s) + r_t;
			//forwarding
			if (if_hazard() == true)
				reg(EXMEM.rd) = EXMEM.temp;
		}
		if (strcmp(IDEX.op, "SUBI") == 0)
		{
			sscanf(IDEX.inst, "$%ld, $%ld, %ld", &r_d, &r_s, &r_t);
			EXMEM.rd = r_d;
			EXMEM.temp = reg(r_s) - r_t;
			//forwarding
			if (if_hazard() == true)
				reg(EXMEM.rd) = EXMEM.temp;
		}
		if (strcmp(IDEX.op, "OR") == 0)
		{
			sscanf(IDEX.inst, "$%ld, $%ld, $%ld", &r_d, &r_s, &r_t);
			EXMEM.rd = r_d;
			EXMEM.temp = reg(r_s) | reg(r_t);
			//forwarding
			if (if_hazard() == true)
				reg(EXMEM.rd) = EXMEM.temp;
		}
		if (strcmp(IDEX.op, "AND") == 0)
		{
			sscanf(IDEX.inst, "$%ld, $%ld, $%ld", &r_d, &r_s, &r_t);
			EXMEM.rd = r_d;
			EXMEM.temp = reg(r_s) & reg(r_t);
			//forwarding
			if (if_hazard() == true)
				reg(EXMEM.rd) = EXMEM.temp;
		}
		if (strcmp(IDEX.op, "SLL") == 0)
		{
			sscanf(IDEX.inst, "$%ld, $%ld, %ld", &r_d, &r_s, &r_t);
			EXMEM.rd = r_d;
			EXMEM.temp = reg(r_s) << r_t;
			//forwarding
			if (if_hazard() == true)
				reg(EXMEM.rd) = EXMEM.temp;
		}
		if (strcmp(IDEX.op, "SRL") == 0)
		{
			sscanf(IDEX.inst, "$%ld, $%ld, %ld", &r_d, &r_s, &r_t);
			EXMEM.rd = r_d;
			EXMEM.temp = reg(r_s) >> r_t;
			//forwarding
			if (if_hazard() == true)
				reg(EXMEM.rd) = EXMEM.temp;
		}
		if ((strcmp(IDEX.op, "SW") == 0) || (strcmp(IDEX.op, "SBU") == 0))
		{
			sscanf(IDEX.inst, "$%ld, %ld($%ld)", &r_d, &r_s, &r_t);
			EXMEM.rd = r_d;
			EXMEM.temp = (reg(r_t) + (r_s));
			//forwarding
			if (if_hazard() == true)
				reg(EXMEM.rd) = EXMEM.temp;
		}
		if ((strcmp(IDEX.op, "LW") == 0) || (strcmp(IDEX.op, "LBU") == 0))
		{
			sscanf(IDEX.inst, "$%ld, %ld($%ld)", &r_d, &r_s, &r_t);
			EXMEM.rd = r_d;
			EXMEM.temp = (reg(r_t) + (r_s));
			//stall and then forwarding
			if (if_hazard() == true)
			{
				/*reg_update();
				mem_writeback();*/
				return;
			}
		}
		clear_pipeline_register_content(&IDEX);
	}
	return;
}
void mem_writeback(void)
{
	if (pipeline_next_stage_null(MEMWB) == true)
	{
		pass_register_content(&EXMEM, &MEMWB);
		if (strcmp(EXMEM.op, "LW") == 0)
		{
			MEMWB.temp = mem(EXMEM.temp);
			MEMWB.rd = EXMEM.rd;
			//forwarding
			if (if_hazard() == true && MEMWB.rd >= 0)
				reg(MEMWB.rd) = MEMWB.temp;
		}
		if (strcmp(EXMEM.op, "SW") == 0)
		{
			mem(EXMEM.temp) = reg(EXMEM.rd);
		}
		if (strcmp(EXMEM.op, "ADD") == 0)
		{
			MEMWB.rd = EXMEM.rd;
			MEMWB.temp = EXMEM.temp;
			//forwarding
			if(if_hazard() == true && MEMWB.rd >= 0)
				reg(MEMWB.rd) = MEMWB.temp;
		}
		if (strcmp(EXMEM.op, "SUB") == 0)
		{
			MEMWB.rd = EXMEM.rd;
			MEMWB.temp = EXMEM.temp;
			//forwarding
			if (if_hazard() == true && MEMWB.rd >= 0)
				reg(MEMWB.rd) = MEMWB.temp;
		}
		if (strcmp(EXMEM.op, "ADDI") == 0)
		{
			MEMWB.rd = EXMEM.rd;
			MEMWB.temp = EXMEM.temp;
			//forwarding
			if (if_hazard() == true && MEMWB.rd >= 0)
				reg(MEMWB.rd) = MEMWB.temp;
		}
		if (strcmp(EXMEM.op, "SUBI") == 0)
		{
			MEMWB.rd = EXMEM.rd;
			MEMWB.temp = EXMEM.temp;
			//forwarding
			if (if_hazard() == true && MEMWB.rd >= 0)
				reg(MEMWB.rd) = MEMWB.temp;
		}
		if (strcmp(EXMEM.op, "OR") == 0)
		{
			MEMWB.rd = EXMEM.rd;
			MEMWB.temp = EXMEM.temp;
			//forwarding
			if (if_hazard() == true && MEMWB.rd >= 0)
				reg(MEMWB.rd) = MEMWB.temp;
		}
		if (strcmp(EXMEM.op, "AND") == 0)
		{
			MEMWB.rd = EXMEM.rd;
			MEMWB.temp = EXMEM.temp;
			//forwarding
			if (if_hazard() == true && MEMWB.rd >= 0)
				reg(MEMWB.rd) = MEMWB.temp;
		}
		if (strcmp(EXMEM.op, "SLL") == 0)
		{
			MEMWB.rd = EXMEM.rd;
			MEMWB.temp = EXMEM.temp;
			//forwarding
			if (if_hazard() == true && MEMWB.rd >= 0)
				reg(MEMWB.rd) = MEMWB.temp;
		}
		if (strcmp(EXMEM.op, "SRL") == 0)
		{
			MEMWB.rd = EXMEM.rd;
			MEMWB.temp = EXMEM.temp;
			//forwarding
			if (if_hazard() == true && MEMWB.rd >= 0)
				reg(MEMWB.rd) = MEMWB.temp;
		}
		if (strcmp(EXMEM.op, "LBU") == 0)
		{
			MEMWB.temp = mem(EXMEM.temp) & 0xFF;
			MEMWB.rd = EXMEM.rd;
			//forwarding
			if (if_hazard() == true && MEMWB.rd >= 0)
				reg(MEMWB.rd) = MEMWB.temp;
		}
		if (strcmp(EXMEM.op, "SBU") == 0)
		{
			mem(EXMEM.temp) = reg(EXMEM.rd) & 0xFF;
			//forwarding
			if (if_hazard() == true && MEMWB.rd >= 0)
				reg(MEMWB.rd) = MEMWB.temp;
		}

		clear_pipeline_register_content(&EXMEM);
	}
	return;
}
void reg_update(void)
{
	if (strcmp(MEMWB.op, "ADD") == 0)
	{
		reg(MEMWB.rd) = MEMWB.temp;
	}

	else if (strcmp(MEMWB.op, "SUB") == 0)
	{
		reg(MEMWB.rd) = MEMWB.temp;
	}

	else if (strcmp(MEMWB.op, "ADDI") == 0)
	{
		reg(MEMWB.rd) = MEMWB.temp;
	}

	else if (strcmp(MEMWB.op, "SUBI") == 0)
	{
		reg(MEMWB.rd) = MEMWB.temp;
	}

	else if (strcmp(MEMWB.op, "OR") == 0)
	{
		reg(MEMWB.rd) = MEMWB.temp;
	}

	else if (strcmp(MEMWB.op, "AND") == 0)
	{
		reg(MEMWB.rd) = MEMWB.temp;
	}

	else if (strcmp(MEMWB.op, "SLL") == 0)
	{
		reg(MEMWB.rd) = MEMWB.temp;
	}

	else if (strcmp(MEMWB.op, "SRL") == 0)
	{
		reg(MEMWB.rd) = MEMWB.temp;
	}

	else if (strcmp(MEMWB.op, "LW") == 0)
	{
		reg(MEMWB.rd) = MEMWB.temp;
	}

	else if (strcmp(MEMWB.op, "LBU") == 0)
	{
		reg(MEMWB.rd) = MEMWB.temp;
	}

	else if (strcmp(MEMWB.op, "SBU") == 0)
	{
		reg(MEMWB.rd) = MEMWB.temp;
	}

	else if (strcmp(MEMWB.op, "SLT") == 0)
	{
		reg(MEMWB.rd) = MEMWB.temp;
	}
	clear_pipeline_register_content(&MEMWB);
	return;
}
void flush_pipeline(void)
{
	return;
}
bool if_hazard(void)   //HOMEWORK
{
	if (EXMEM.rd == -1 || IDEX.rs == -1)
		return false;
	if (MEMWB.rd == -1 || IDEX.rt == -1)
		return false;
	if (EXMEM.rd == IDEX.rs)	//1a
	{
		printf("1a\n");
		return true;
	}
		
	else if (EXMEM.rd == IDEX.rt)	//1b
	{
		printf("1b\n");
		return true;
	}
	else if (MEMWB.rd == IDEX.rs)	//2a
	{
		printf("2a\n");
		return true;
	}
	else if (MEMWB.rd == IDEX.rt)	//2b
	{
		printf("2b\n");
		return true;
	}
	else
		return false;

	//return true;
	//1a : EX / MEM.RegisterRd = ID / EX.RegisterRs
	//1b : EX / MEM.RegisterRd = ID / EX.RegisterRt
	//2a : MEM / WB.RegisterRd = ID / EX.RegisterRs
	//2b : MEM / WB.RegisterRd = ID / EX.RegisterRt
}
bool pipeline_null(void)
{
	if (DATAPATH_TYPE == PIPELINE)
	{
		if (pipeline_next_stage_null(IFID) == true && pipeline_next_stage_null(IDEX) == true && 
			pipeline_next_stage_null(EXMEM) == true && pipeline_next_stage_null(MEMWB) == true)
			return false;
		else
			return true;
	}
	else
	{
		return false;
	}
}
void print_pipeline_register_content(void)
{
	printf("IFID\n");
	printf("rs: $%d rt: $%d rd: $%d imm: %d temp: %d\n", IFID.rs, IFID.rt, IFID.rd, IFID.imm, IFID.temp);
	printf("op: %s inst: %s\n", IFID.op, IFID.inst);
	printf("IDEX\n");
	printf("rs: $%d rt: $%d rd: $%d imm: %d temp: %d\n", IDEX.rs, IDEX.rt, IDEX.rd, IDEX.imm, IDEX.temp);
	printf("op: %s inst: %s\n", IDEX.op, IDEX.inst);
	printf("EXMEM\n");
	printf("rs: $%d rt: $%d rd: $%d imm: %d temp: %d\n", EXMEM.rs, EXMEM.rt, EXMEM.rd, EXMEM.imm, EXMEM.temp);
	printf("op: %s inst: %s\n", EXMEM.op, EXMEM.inst);
	printf("MEMWB\n");
	printf("rs: $%d rt: $%d rd: $%d imm: %d temp: %d\n", MEMWB.rs, MEMWB.rt, MEMWB.rd, MEMWB.imm, MEMWB.temp);
	printf("op: %s inst: %s\n", MEMWB.op, MEMWB.inst);
	printf("\n");
	//printf("IFID IDEX EXMEM MEMWB\n $%d imm: %d temp: %d\n", IFID.rs, IFID.rt, IFID.rd, IFID.imm, IFID.temp);
	return;

}
void clear_pipeline_register_content(PIPELINE_MEMORY *temp)
{
	temp->rs = -1;
	temp->rt = -1;
	temp->rd = -1;
	temp->imm = -1;
	temp->temp = -1;
	strcpy(temp->op, "null");
	strcpy(temp->inst, "null");
	return;
}
void pass_register_content(PIPELINE_MEMORY *source, PIPELINE_MEMORY *destination)
{
	destination->rs = source->rs;
	destination->rt = source->rt;
	destination->rd = source->rd;
	destination->imm = source->imm;
	destination->temp = source->temp;
	strcpy(destination->op, source->op);
	strcpy(destination->inst, source->inst);
	return;

}
bool pipeline_next_stage_null(PIPELINE_MEMORY temp)
{
	if (temp.rs == -1 && temp.rt == -1 && temp.rd == -1 && temp.imm == -1 && 
		strcmp(temp.op, "null") == 0 && strcmp(temp.inst, "null") == 0)
		return true;
	return false;
}
