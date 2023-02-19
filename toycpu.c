/*
 * @author Tohar
 * 8-bit CPU
 */

#include <stdint.h>
#include <stdio.h>

typedef struct
{
	uint8_t reg_a;
	uint8_t reg_b;
} CPU;

void init_cpu(CPU *cpu, uint8_t reg_a, uint8_t reg_b)
{
	cpu->reg_a = reg_a;
	cpu->reg_b = reg_b;
}

void set_reg_a(CPU *cpu, uint8_t val)
{
	cpu->reg_a = val;
}

void set_reg_b(CPU *cpu, uint8_t val) 
{
	cpu->reg_b = val;
}

void setmem_reg_a(CPU *cpu, int mem[], int pos)
{
	cpu->reg_a = mem[pos];
}

void memset_reg_a(CPU *cpu, int *mem[], int pos) 
{
	mem[(int)mem[pos+1]] = cpu->reg_a;
}

void add(CPU *cpu)
{
	cpu->reg_a = cpu->reg_a + cpu->reg_b;
}

void sub(CPU *cpu) 
{
	cpu->reg_a = cpu->reg_a - cpu->reg_b;
}

int jmp(CPU *cpu, uint8_t pos)
{
	return (int) pos;
}

int jz(CPU *cpu, int curr, uint8_t pos)
{
	if (cpu->reg_a == 0) {
		return (int) pos-1;
	}
	return curr;
}

void print_cpu(CPU *cpu)
{
	printf("Register A: %d\tRegister B: %d\n", cpu->reg_a, cpu->reg_b);
}

void print_char(CPU *cpu) 
{
	printf("%c\n", (char) cpu->reg_a);
}

void exec_mem(CPU *cpu, int mem[]) 
{
	int len = sizeof(mem)/sizeof(mem[0]);
	int i = 0;
	for (int i = 0; i < len; i++) {
		int val = mem[i];
		switch(val) {
			case 1:
				set_reg_a(cpu, (uint8_t) mem[i+1]);
				i+=1;
				break;
			case 2:
				set_reg_b(cpu, (uint8_t) mem[i+1]);
				i+=1;
				break;
			case 3:
				setmem_reg_a(cpu, mem, i+1);
				i+=1;
				break;
			case 4:
				memset_reg_a(cpu, &mem, i);
				i+=1;
				break;
			case 5:
				add(cpu);
				break;
			case 6:
				sub(cpu);
				break;
			case 7:
				i = jmp(cpu, (uint8_t) mem[i+1]);
				break;
			case 8:
				i = jz(cpu, i, (uint8_t) mem[i+1]);
				break;
			case 9:
				print_cpu(cpu);
				break;
			case 10:
				print_char(cpu);
				break;
			case 11:
				return;
			default:
				printf("Error - Invalid Instruction");
				return;
		}
	}
}

int main(int argc, char **argv) 
{
	int mem[] = 
	{
		2, 1,  // Set reg_b to 1
		3, 15, // Set reg_a to pos 15
		8, 14, // jz 14
		10,    // print reg_a
		3, 3,  // Set A to pos 3
		5,     // reg_a + 1
		4, 3,  // Set pos 3 to reg_a
		7, 2,  // jmp 2
		11,    // Halt if zero
		72, 101, 108, 108, 111, 32, 87, 111, 114, 108, 100, // "Hello World"
	};
	CPU cpu = {0, 0};
	exec_mem(&cpu, mem);
	printf("CPU Initialized Correctly & Memory Executed.\n");
}
	
