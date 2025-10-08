#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

uint32_t SR(uint32_t sr, uint32_t cy1, uint32_t cy2, uint32_t iv, uint32_t ov1, uint32_t ov2, uint32_t sn, uint32_t zd, uint32_t zn, int toma) {
	if(toma==89){
		if(cy1==1)
			sr = sr | (1 << 0); //CY1
		else if(cy2!=0)
			sr = sr | (1 << 0); //CY2
		else
			sr = sr & ~(1 << 0);
		if(ov1==1)
			sr = sr | (1 << 3); //OV1
		else if(ov2!=0)
			sr = sr | (1 << 3); //OV2
		else
			sr = sr & ~(1 << 3);
		if(sn==1)
			sr = sr | (1 << 4); //SN
		else
			sr = sr & ~(1 << 4);
		if(zn)
			sr = sr | (1 << 6); //ZN
		else
			sr = sr & ~(1 << 6);
	}
	if(toma==65) {
		if(cy1==1)
			sr = sr | (1 << 0); //CY1
		else if(cy2!=0)
			sr = sr | (1 << 0); //CY2
		else
			sr = sr & ~(1 << 0);
		if(zn)
			sr = sr | (1 << 6); //ZN
		else
			sr = sr & ~(1 << 6);
	}
	if(toma==72) {
		if(ov1==1)
			sr = sr | (1 << 3); //OV1
		else
			sr = sr & ~(1 << 3);
		if(ov2!=0)
			sr = sr | (1 << 3); //OV2
		else
			sr = sr & ~(1 << 3);
		if(zn)
			sr = sr | (1 << 6); //ZN
		else
			sr = sr & ~(1 << 6);
	}
	if(toma==97) {
		if(cy1==1)
			sr = sr | (1 << 0); //CY1
		else if(cy2!=0)
			sr = sr | (1 << 0); //CY2
		else
			sr = sr & ~(1 << 0);
		if(zd)
			sr = sr | (1 << 5); //ZD
		else
			sr = sr & ~(1 << 5);
		if(zn)
			sr = sr | (1 << 6); //ZN
		else
			sr = sr & ~(1 << 6);
	}
	if(toma==104) {
		if(ov1==1)
			sr = sr | (1 << 3); //OV1
		else if(ov2!=0)
			sr = sr | (1 << 3); //OV2
		else
			sr = sr & ~(1 << 3);
		if(zd)
			sr = sr | (1 << 5); //ZD
		else
			sr = sr & ~(1 << 5);
		if(zn) 
			sr = sr | (1 << 6); //ZN
		else
			sr = sr & ~(1 << 6);
	}
	if(toma==80) {
		if(sn)
			sr = sr | (1 << 4); //SN
		else
			sr = sr & ~(1 << 4);
		if(zn)
			sr = sr | (1 << 6); //ZN
		else
			sr = sr & ~(1 << 6);
	}
	return sr;
}

void idRegistrador(char reg[10], uint32_t x){
	if(x==28) 
		sprintf(reg, "ir");
	else if(x==29) 
		sprintf(reg, "pc");
	else if(x==30) 
		sprintf(reg, "sp");
	else if(x==31) 
		sprintf(reg, "sr");
	else 
		sprintf(reg, "r%u", x);
}

void idRegistradorM(char reg[10], uint32_t x) {
	if(x==28) 
		sprintf(reg, "IR");
	else if(x==29) 
		sprintf(reg, "PC");
	else if(x==30) 
		sprintf(reg, "SP");
	else if(x==31) 
		sprintf(reg, "SR");
	else 
		sprintf(reg, "R%u", x);
}

// Função principal
int main(int argc, char* argv[]) {
	// Exibindo a quantidade de argumentos
	printf("Quantidade de argumentos (argc): %i\n", argc);
	// Iterando sobre o(s) argumento(s) do programa
	for(uint32_t i = 0; i < argc; i++) {
		// Mostrando o argumento i
		printf("Argumento %i (argv[%i]): %s\n", i, i, argv[i]);
	}
	// Abrindo os arquivos com as permissoes corretas
	FILE* input = fopen("poxim1_input.txt", "r");
	FILE* output = fopen("poxim1_output.txt", "w");
	if(input == NULL) {
		fprintf(output, "Nao foi possivel ler o arquivo\n");
		return 1;
	}
	// 32 registradores inicializados com zero
	uint32_t R[32] = { 0 };
	// Memoria com 32 KiB inicializada com zero
	uint8_t* MEM8 = (uint8_t*)(calloc(32, 1024));
	// Atribuindo os valores da memória
	uint32_t cont;
	uint32_t j=0;
	while (fscanf(input, "%" SCNx32, &cont) == 1 && j < 8 * 1024) {
        MEM8[j * sizeof(uint32_t) + 0] = (uint8_t)((cont >> 24) & 0xFF); // Byte mais significativo
        MEM8[j * sizeof(uint32_t) + 1] = (uint8_t)((cont >> 16) & 0xFF);
        MEM8[j * sizeof(uint32_t) + 2] = (uint8_t)((cont >> 8) & 0xFF);
        MEM8[j * sizeof(uint32_t) + 3] = (uint8_t)(cont & 0xFF); // Byte menos significativo
        j++;
    }
	// Exibindo a inicializacao da execucao
	fprintf(output, "[START OF SIMULATION]\n");
	// Setando a condicao de execucao para verdadeiro
	uint8_t executa = 1;
	// Enquanto executa for verdadeiro
	while(executa) {
		// Cadeia de caracteres da instrucao
		char instrucao[30] = {0};
		char reg1[10] = {0}, reg2[10] = {0}, reg3[10] = {0}, reg4[10] = {0}, reg5[10] = {0};
		char reg1out[10] = {0}, reg2out[10] = {0}, reg3out[10] = {0}, reg4out[10] = {0}, reg5out[10] = {0};
		// Declarando operandos
		uint8_t z = 0, x = 0, y = 0, l = 0, w = 0, v = 0, aux = 0, aux2 = 0;
		uint32_t pc = 0, sp = 0, i = 0, xyl = 0;
		uint64_t u64 = 0;
		int64_t i64 = 0;
		// Carregando a instrucao de 32 bits (4 bytes) da memoria indexada pelo PC (R29) no registrador IR (R28)
		R[28] = ((MEM8[R[29] + 0] << 24) | (MEM8[R[29] + 1] << 16) | (MEM8[R[29] + 2] << 8) | (MEM8[R[29] + 3] << 0));
		// Obtendo o codigo da operacao (6 bits mais significativos)
		uint8_t opcode = (R[28] & (0b111111 << 26)) >> 26;
		uint8_t subopcode = (R[28] & (0b111 << 8)) >> 8;
		// Decodificando a instrucao buscada na memoria
		switch(opcode) {
			//mov
			case 0b000000:
				if(R[28]!=0){
					// Obtendo operandos
					z = (R[28] & (0b11111 << 21)) >> 21;
					xyl = R[28] & 0x1FFFFF;
					// Execucao do comportamento
					R[z] = xyl;
					// Formatacao da instrucao
					idRegistrador(reg1, (uint32_t)z);
					sprintf(instrucao, "mov %s,%u", reg1, xyl);
					// Formatacao de saida em arquivo
					idRegistradorM(reg1out, (uint32_t)z);
					fprintf(output, "0x%08X:\t%-25s\t%s=0x%08X\n", R[29], instrucao, reg1out, xyl);
				}
				break;
			//movs
			case 0b000001:
				// Obtendo operandos
				z = (R[28] & (0b11111 << 21)) >> 21;
				xyl = R[28] & 0x1FFFFF;
				// Execucao do comportamento
				if(((R[28] & (0b1 << 20)) >> 20) == 1)
					xyl = xyl | (0b11111111111 << 21);
				R[z] = (int32_t)xyl;
				// Formatacao da instrucao
				idRegistrador(reg1, (uint32_t)z);
				sprintf(instrucao, "movs %s,%d", reg1, (int32_t)xyl);
				// Formatacao de saida em arquivo
				idRegistradorM(reg1out, (uint32_t)z);
				fprintf(output, "0x%08X:\t%-25s\t%s=0x%08X\n", R[29], instrucao, reg1out, R[z]);
				break;
			//add
			case 0b000010:
				// Obtendo operandos
				y = (R[28] & (0b11111 << 11)) >> 11;
				x = (R[28] & (0b11111 << 16)) >> 16;
				z = (R[28] & (0b11111 << 21)) >> 21;
				// Execucao do comportamento
				int64_t add = (int64_t)R[x] + (int64_t)R[y];
				R[z] = (uint32_t)add;
				// Atualizando SR
				R[31] = SR(R[31], ((add >> 32) & 0b1), 0, 0, ((((R[x] & (0b1 << 31)) >> 31)==((R[y] & (0b1 << 31)) >> 31)) && (((R[z] & (0b1 << 31)) >> 31)!=((R[x] & (0b1 << 31)) >> 31))), 0, ((R[z] & (0b1 << 31)) >> 31), 0, (R[z]==0), 89);
				// Formatação da instrução
				idRegistrador(reg1, (uint32_t)z);
				idRegistrador(reg2, (uint32_t)x);
				idRegistrador(reg3, (uint32_t)y);
				sprintf(instrucao, "add %s,%s,%s", reg1, reg2, reg3);
				// Formatação de saida em arquivo
				idRegistradorM(reg1out, (uint32_t)z);
				idRegistradorM(reg2out, (uint32_t)x);
				idRegistradorM(reg3out, (uint32_t)y);
				fprintf(output, "0x%08X:\t%-25s\t%s=%s+%s=0x%08X,SR=0x%08X\n", R[29], instrucao, reg1out, reg2out, reg3out, R[z], R[31]);
				break;
			//sub
			case 0b000011:
				// Obtendo operandos
				y = (R[28] & (0b11111 << 11)) >> 11;
				x = (R[28] & (0b11111 << 16)) >> 16;
				z = (R[28] & (0b11111 << 21)) >> 21;
				// Execucao do comportamento
				int64_t sub = (int64_t)R[x] - (int64_t)R[y];
				R[z] = (uint32_t)sub;
				// Atualizando SR
				R[31] = SR(R[31], ((sub >> 32) & 0b1), 0, 0, ((((R[x] & (0b1 << 31)) >> 31)!=((R[y] & (0b1 << 31)) >> 31)) && (((R[z] & (0b1 << 31)) >> 31)!=((R[x] & (0b1 << 31)) >> 31))), 0, ((R[z] & (0b1 << 31)) >> 31), 0, (R[z]==0), 89);
				idRegistrador(reg1, (uint32_t)z);
				idRegistrador(reg2, (uint32_t)x);
				idRegistrador(reg3, (uint32_t)y);
				sprintf(instrucao, "sub %s,%s,%s", reg1, reg2, reg3);
				// Formatação de saida em arquivo
				idRegistradorM(reg1out, (uint32_t)z);
				idRegistradorM(reg2out, (uint32_t)x);
				idRegistradorM(reg3out, (uint32_t)y);
				fprintf(output, "0x%08X:\t%-25s\t%s=%s-%s=0x%08X,SR=0x%08X\n", R[29], instrucao, reg1out, reg2out, reg3out, R[z], R[31]);
				break;
			//mul, sll, muls, sla, div, srl, divs, sra
			case 0b000100:
				switch(subopcode) {
					//mul
					case 0b000:
						// Obtendo operandos
						l = R[28] & 0b11111;
						y = (R[28] & (0b11111 << 11)) >> 11;
						x = (R[28] & (0b11111 << 16)) >> 16;
						z = (R[28] & (0b11111 << 21)) >> 21;
						// Execucao do comportamento
						uint64_t mul = (uint64_t)R[x] * (uint64_t)R[y];
						R[l] = (uint32_t)(mul >> 32);
						R[z] = (uint32_t)mul;
						// Atualizando SR
						R[31] = SR(R[31], 0, R[l], 0, 0, 0, 0, 0, (mul==0), 65);
						// Formatacao da instrucao
						idRegistrador(reg1, (uint32_t)l);
						idRegistrador(reg2, (uint32_t)z);
						idRegistrador(reg3, (uint32_t)x);
						idRegistrador(reg4, (uint32_t)y);
						sprintf(instrucao, "mul %s,%s,%s,%s", reg1, reg2, reg3, reg4);
						// Formatacao de saida em arquivo
						idRegistradorM(reg1out, (uint32_t)l);
						idRegistradorM(reg2out, (uint32_t)z);
						idRegistradorM(reg3out, (uint32_t)x);
						idRegistradorM(reg4out, (uint32_t)y);
						fprintf(output, "0x%08X:\t%-25s\t%s:%s=%s*%s=0x%08X%08X,SR=0x%08X\n", R[29], instrucao, reg1out, reg2out, reg3out, reg4out, R[l], R[z], R[31]);
						break;
					//sll
					case 0b001:
						// Obtendo operandos
						l = R[28] & 0b11111;
						y = (R[28] & (0b11111 << 11)) >> 11;
						x = (R[28] & (0b11111 << 16)) >> 16;
						z = (R[28] & (0b11111 << 21)) >> 21;
						// Execucao do comportamento
						u64 = ((uint64_t)R[z] << 32) | R[y];
						u64 = u64 << (l+1);
						R[x] = (uint32_t)u64;
						R[z] = (uint32_t)(u64 >> 32);
						// Atualizando SR
						R[31] = SR(R[31], 0, R[z], 0, 0, 0, 0, 0, (u64==0), 65);
						// Formatacao da instrucao
						idRegistrador(reg1, (uint32_t)z);
						idRegistrador(reg2, (uint32_t)x);
						idRegistrador(reg3, (uint32_t)y);
						sprintf(instrucao, "sll %s,%s,%s,%u", reg1, reg2, reg3, l);
						// Formatacao de saida em arquivo
						idRegistradorM(reg1out, (uint32_t)z);
						idRegistradorM(reg2out, (uint32_t)x);
						idRegistradorM(reg3out, (uint32_t)y);
						fprintf(output, "0x%08X:\t%-25s\t%s:%s=%s:%s<<%u=0x%08X%08X,SR=0x%08X\n", R[29], instrucao, reg1out, reg2out, reg1out, reg3out, l+1, R[z], R[x], R[31]);
						break;
					//muls
					case 0b010:
						// Obtendo operandos
						l = R[28] & 0b11111;
						y = (R[28] & (0b11111 << 11)) >> 11;
						x = (R[28] & (0b11111 << 16)) >> 16;
						z = (R[28] & (0b11111 << 21)) >> 21;
						aux = (R[28] & (0b1 << 31)) >> 31;
						// Execucao do comportamento
						int64_t muls = (int32_t)R[x] * (int32_t)R[y];
						R[z] = (uint32_t)muls;
						R[l] = (uint32_t)(muls >> 32);
						// Atualizando SR
						R[31] = SR(R[31], 0, 0, 0, 0, R[l], 0, 0, (muls==0), 72);
						// Formatacao da instrucao
						idRegistrador(reg1, (uint32_t)l);
						idRegistrador(reg2, (uint32_t)z);
						idRegistrador(reg3, (uint32_t)x);
						idRegistrador(reg4, (uint32_t)y);
						sprintf(instrucao, "muls %s,%s,%s,%s", reg1, reg2, reg3, reg4);
						// Formatacao de saida em arquivo
						idRegistradorM(reg1out, (uint32_t)l);
						idRegistradorM(reg2out, (uint32_t)z);
						idRegistradorM(reg3out, (uint32_t)x);
						idRegistradorM(reg4out, (uint32_t)y);
						fprintf(output, "0x%08X:\t%-25s\t%s:%s=%s*%s=0x%08X%08X,SR=0x%08X\n", R[29], instrucao, reg1out, reg2out, reg3out, reg4out, R[l], R[z], R[31]);
						break;
					//sla
					case 0b011:
						// Obtendo operandos
						l = R[28] & 0b11111;
						y = (R[28] & (0b11111 << 11)) >> 11;
						x = (R[28] & (0b11111 << 16)) >> 16;
						z = (R[28] & (0b11111 << 21)) >> 21;
						aux = (R[28] & (0b1 << 5)) >> 5;
						// Execucao do comportamento
						if(aux == 1)
							l = l | (0b11111111111111111111111111 << 6);
						i64 = ((int64_t)R[z] << 32) | R[y];
						i64 = i64 << (l+1);
						R[x] = (uint32_t)i64;
						// Atualizando SR
						R[31] = SR(R[31], 0, 0, 0, 0, R[z], 0, 0, R[z]==0, 72);
						R[31] = SR(R[31], 0, 0, 0, 0, R[z], 0, 0, R[y]==0, 72);
						// Formatacao da instrucao
						idRegistrador(reg1, (uint32_t)z);
						idRegistrador(reg2, (uint32_t)x);
						idRegistrador(reg3, (uint32_t)y);
						sprintf(instrucao, "sla %s,%s,%s,%u", reg1, reg2, reg3, l);
						// Formatacao de saida em arquivo
						idRegistradorM(reg1out, (uint32_t)z);
						idRegistradorM(reg2out, (uint32_t)x);
						idRegistradorM(reg3out, (uint32_t)y);
						fprintf(output, "0x%08X:\t%-25s\t%s:%s=%s:%s<<%u=0x%08X%08X,SR=0x%08X\n", R[29], instrucao, reg1out, reg2out, reg1out, reg3out, l+1, (int32_t)R[z], (int32_t)R[x], R[31]);
						break;
					//div
					case 0b100:
						// Obtendo operandos
						l = R[28] & 0b11111;
						y = (R[28] & (0b11111 << 11)) >> 11;
						x = (R[28] & (0b11111 << 16)) >> 16;
						z = (R[28] & (0b11111 << 21)) >> 21;
						// Execucao do comportamento
						if(R[y]!=0) {
							uint64_t mod = R[x]%R[y];
							R[l] = mod;
							uint64_t div = R[x]/R[y];
							R[z] = div;
						}
						// Atualizando SR
						R[31] = SR(R[31], 0, R[l], 0, 0, 0, 0, (R[y]==0), (R[z]==0), 97);
						// Formatacao da instrucao
						idRegistrador(reg1, (uint32_t)l);
						idRegistrador(reg2, (uint32_t)z);
						idRegistrador(reg3, (uint32_t)x);
						idRegistrador(reg4, (uint32_t)y);
						sprintf(instrucao, "div %s,%s,%s,%s", reg1, reg2, reg3, reg4);
						// Formatacao de saida em arquivo
						idRegistradorM(reg1out, (uint32_t)l);
						idRegistradorM(reg2out, (uint32_t)z);
						idRegistradorM(reg3out, (uint32_t)x);
						idRegistradorM(reg4out, (uint32_t)y);
						fprintf(output, "0x%08X:\t%-25s\t%s=%s%%%s=0x%08X,%s=%s/%s=0x%08X,SR=0x%08X\n", R[29], instrucao, reg1out, reg3out, reg4out, R[l], reg2out, reg3out, reg4out, R[z], R[31]);
						break;
					//srl
					case 0b101:
						// Obtendo operandos
						l = R[28] & 0b11111;
						y = (R[28] & (0b11111 << 11)) >> 11;
						x = (R[28] & (0b11111 << 16)) >> 16;
						z = (R[28] & (0b11111 << 21)) >> 21;
						// Execucao do comportamento
						u64 = ((uint64_t)R[z] << 32) | R[y];
						u64 = u64 >> (l+1);
						R[x] = (uint32_t)u64;
						R[z] = (uint32_t)(u64 >> 32);
						// Atualizando SR
						R[31] = SR(R[31], 0, R[z], 0, 0, 0, 0, 0, (u64==0), 65);
						// Formatacao da instrucao
						idRegistrador(reg1, (uint32_t)z);
						idRegistrador(reg2, (uint32_t)x);
						idRegistrador(reg3, (uint32_t)y);
						sprintf(instrucao, "srl %s,%s,%s,%u", reg1, reg2, reg3, l);
						// Formatacao de saida em arquivo
						idRegistradorM(reg1out, (uint32_t)z);
						idRegistradorM(reg2out, (uint32_t)x);
						idRegistradorM(reg3out, (uint32_t)y);
						fprintf(output, "0x%08X:\t%-25s\t%s:%s=%s:%s>>%u=0x%08X%08X,SR=0x%08X\n", R[29], instrucao, reg1out, reg2out, reg1out, reg3out, l+1, R[z], R[x], R[31]);
						break;
					//divs
					case 0b110:
						// Obtendo operandos
						l = R[28] & 0b11111;
						y = (R[28] & (0b11111 << 11)) >> 11;
						x = (R[28] & (0b11111 << 16)) >> 16;
						z = (R[28] & (0b11111 << 21)) >> 21;
						// Execucao do comportamento
						if(R[y]!=0) {
							int32_t mods = (int32_t)R[x] % (int32_t)R[y];
							R[l] = (uint32_t)mods;
							int32_t divs = (int32_t)R[x] / (int32_t)R[y];
							R[z] = (uint32_t)divs;
						}
						// Atualizando SR
						R[31] = SR(R[31], 0, 0, 0, 0, R[l], 0, (R[y]==0), (R[z]==0), 104);
						// Formatacao da instrucao
						idRegistrador(reg1, (uint32_t)l);
						idRegistrador(reg2, (uint32_t)z);
						idRegistrador(reg3, (uint32_t)x);
						idRegistrador(reg4, (uint32_t)y);
						sprintf(instrucao, "divs %s,%s,%s,%s", reg1, reg2, reg3, reg4);
						// Formatacao de saida em arquivo
						idRegistradorM(reg1out, (uint32_t)l);
						idRegistradorM(reg2out, (uint32_t)z);
						idRegistradorM(reg3out, (uint32_t)x);
						idRegistradorM(reg4out, (uint32_t)y);
						fprintf(output, "0x%08X:\t%-25s\t%s=%s%%%s=0x%08X,%s=%s/%s=0x%08X,SR=0x%08X\n", R[29], instrucao, reg1out, reg3out, reg4out, (int32_t)R[l], reg2out, reg3out, reg4out, (int32_t)R[z], R[31]);
						break;
					//sra
					case 0b111:
						// Obtendo operandos
						l = R[28] & 0b11111;
						y = (R[28] & (0b11111 << 11)) >> 11;
						x = (R[28] & (0b11111 << 16)) >> 16;
						z = (R[28] & (0b11111 << 21)) >> 21;
						aux = (R[28] & (0b1 << 5)) >> 5;
						// Execucao do comportamento
						if(aux == 1)
							l = l | (0b11111111111111111111111111 << 6);	
						i64 = ((int64_t)R[z] << 32) | R[y];
						i64 = i64 >> (l+1);
						R[x] = (uint32_t)i64;
						R[z] = (uint32_t)(i64 >> 32);
						// Atualizando SR
						R[31] = SR(R[31], 0, 0, 0, 0, R[z], 0, 0, (i64==0), 72);
						// Formatacao da instrucao
						idRegistrador(reg1, (uint32_t)z);
						idRegistrador(reg2, (uint32_t)x);
						idRegistrador(reg3, (uint32_t)y);
						sprintf(instrucao, "sra %s,%s,%s,%u", reg1, reg2, reg3, l);
						// Formatacao de saida em arquivo
						idRegistradorM(reg1out, (uint32_t)z);
						idRegistradorM(reg2out, (uint32_t)x);
						idRegistradorM(reg3out, (uint32_t)y);
						fprintf(output, "0x%08X:\t%-25s\t%s:%s=%s:%s>>%u=0x%08X%08X,SR=0x%08X\n", R[29], instrucao, reg1out, reg2out, reg1out, reg3out, l+1, R[z], R[x], R[31]);
						break;
					//Instrucao desconhecida
					default:
						// Exibindo mensagem de erro
						fprintf(output, "Instrucao desconhecida!\n");
						// Parar a execucao
						executa = 0;
						break;
				}
				break;
			//cmp
			case 0b000101:
				// Obtendo operandos
				y = (R[28] & (0b11111 << 11)) >> 11;
				x = (R[28] & (0b11111 << 16)) >> 16;
				// Execucao do comportamento
				int64_t cmp = (int64_t)R[x] - (int64_t)R[y];
				// Atualizando SR
				R[31] = SR(R[31], ((cmp >> 32) & 0b1), 0, 0, ((((R[x] & (0b1 << 31)) >> 31)!=((R[y] & (0b1 << 31)) >> 31))&&(((cmp & (0b1 << 31)) >> 31)!=((R[x] & (0b1 << 31)) >> 31))), 0, (((cmp & (0b1 << 31)) >> 31) & 0b1), 0, (cmp==0), 89);
				// Formatacao da instrucaoxvh
				idRegistrador(reg1, (uint32_t)x);
				idRegistrador(reg2, (uint32_t)y);
				sprintf(instrucao, "cmp %s,%s", reg1, reg2);
				// Formatacao de saida em arquivo
				fprintf(output, "0x%08X:\t%-25s\tSR=0x%08X\n", R[29], instrucao, R[31]);
				break;
			//and
			case 0b000110:
				// Obtendo operandos
				y = (R[28] & (0b11111 << 11)) >> 11;
				x = (R[28] & (0b11111 << 16)) >> 16;
				z = (R[28] & (0b11111 << 21)) >> 21;
				// Execucao do comportamento
				R[z] = R[x] & R[y];
				// Atualizando SR
				R[31] = SR(R[31], 0, 0, 0, 0, 0, ((R[z] & (0b1 << 31)) >> 31), 0, (R[z]==0), 80);
				// Formatacao da instrucao
				idRegistrador(reg1, (uint32_t)z);
				idRegistrador(reg2, (uint32_t)x);
				idRegistrador(reg3, (uint32_t)y);
				sprintf(instrucao, "and %s,%s,%s", reg1, reg2, reg3);
				// Formatacao de saida em arquivo
				idRegistradorM(reg1out, (uint32_t)z);
				idRegistradorM(reg2out, (uint32_t)x);
				idRegistradorM(reg3out, (uint32_t)y);
				fprintf(output, "0x%08X:\t%-25s\t%s=%s&%s=0x%08X,SR=0x%08X\n", R[29], instrucao, reg1out, reg2out, reg3out, R[z], R[31]);
				break;
			//or
			case 0b000111:
				// Obtendo operandos
				y = (R[28] & (0b11111 << 11)) >> 11;
				x = (R[28] & (0b11111 << 16)) >> 16;
				z = (R[28] & (0b11111 << 21)) >> 21;
				// Execucao do comportamento
				R[z] = R[x] | R[y];
				// Atualizando SR
				R[31] = SR(R[31], 0, 0, 0, 0, 0, ((R[z] & (0b1 << 31)) >> 31), 0, (R[z]==0), 80);
				// Formatacao da instrucao
				idRegistrador(reg1, (uint32_t)z);
				idRegistrador(reg2, (uint32_t)x);
				idRegistrador(reg3, (uint32_t)y);
				sprintf(instrucao, "or %s,%s,%s", reg1, reg2, reg3);
				// Formatacao de saida em arquivo
				idRegistradorM(reg1out, (uint32_t)z);
				idRegistradorM(reg2out, (uint32_t)x);
				idRegistradorM(reg3out, (uint32_t)y);
				fprintf(output, "0x%08X:\t%-25s\t%s=%s|%s=0x%08X,SR=0x%08X\n", R[29], instrucao, reg1out, reg2out, reg3out, R[z], R[31]);
				break;
			//not
			case 0b001000:
				// Obtendo operandos
				x = (R[28] & (0b11111 << 16)) >> 16;
				z = (R[28] & (0b11111 << 21)) >> 21;
				// Execucao do comportamento
				R[z] = ~R[x];
				// Atualizando SR
				R[31] = SR(R[31], 0, 0, 0, 0, 0, ((R[z] & (0b1 << 31)) >> 31), 0, (R[z]==0), 80);
				// Formatacao da instrucao
				idRegistrador(reg1, (uint32_t)z);
				idRegistrador(reg2, (uint32_t)x);
				sprintf(instrucao, "not %s,%s", reg1, reg2);
				// Formatacao de saida em arquivo
				idRegistradorM(reg1out, (uint32_t)z);
				idRegistradorM(reg2out, (uint32_t)x);
				fprintf(output, "0x%08X:\t%-25s\t%s=~%s=0x%08X,SR=0x%08X\n", R[29], instrucao, reg1out, reg2out, R[z], R[31]);
				break;
			//xor
			case 0b001001:
				// Obtendo operandos
				y = (R[28] & (0b11111 << 11)) >> 11;
				x = (R[28] & (0b11111 << 16)) >> 16;
				z = (R[28] & (0b11111 << 21)) >> 21;
				// Execucao do comportamento
				R[z] = R[x] ^ R[y];
				// Atualizando SR
				R[31] = SR(R[31], 0, 0, 0, 0, 0, ((R[z] & (0b1 << 31)) >> 31), 0, (R[z]==0), 80);
				// Formatacao da instrucao
				idRegistrador(reg1, (uint32_t)z);
				idRegistrador(reg2, (uint32_t)x);
				idRegistrador(reg3, (uint32_t)y);
				sprintf(instrucao, "xor %s,%s,%s", reg1, reg2, reg3);
				// Formatacao de saida em arquivo
				idRegistradorM(reg1out, (uint32_t)z);
				idRegistradorM(reg2out, (uint32_t)x);
				idRegistradorM(reg3out, (uint32_t)y);
				fprintf(output, "0x%08X:\t%-25s\t%s=%s^%s=0x%08X,SR=0x%08X\n", R[29], instrucao, reg1out, reg2out, reg3out, R[z], R[31]);
				break;
			//addi
			case 0b010010:
				// Obtendo operandos
				i = R[28] & 0b1111111111111111;
				x = (R[28] & (0b11111 << 16)) >> 16;
				z = (R[28] & (0b11111 << 21)) >> 21;
				aux = (R[28] & (0b1 << 15)) >> 15;
				// Execucao do comportamento
				if(aux == 1)
					i = i | (0b1111111111111111 << 16);
				int64_t addi = (int64_t)R[x] + (int64_t)i;
				R[z] = (uint32_t)addi;
				// Atualizando SR
				R[31] = SR(R[31], (((addi >> 1) & (0b1 << 31)) >> 31), 0, 0, ((((R[x] & (0b1 << 31)) >> 31)==((i & (0b1 << 15)) >> 15)) && (((R[z] & (0b1 << 31)) >> 31)!=((R[x] & (0b1 << 31)) >> 31))), 0, ((R[z] & (0b1 << 31)) >> 31), 0, (R[z]==0), 89);
				// Formatação da instrução
				idRegistrador(reg1, (uint32_t)z);
				idRegistrador(reg2, (uint32_t)x);
				sprintf(instrucao, "addi %s,%s,%d", reg1, reg2, (int32_t)i);
				// Formatação de saida em arquivo
				idRegistradorM(reg1out, (uint32_t)z);
				idRegistradorM(reg2out, (uint32_t)x);
				fprintf(output, "0x%08X:\t%-25s\t%s=%s+0x%08X=0x%08X,SR=0x%08X\n", R[29], instrucao, reg1out, reg2out, i, R[z], R[31]);
				break;
			//subi
			case 0b010011:
				// Obtendo operandos
				i = R[28] & 0b1111111111111111;
				x = (R[28] & (0b11111 << 16)) >> 16;
				z = (R[28] & (0b11111 << 21)) >> 21;
				aux = (R[28] & (0b1 << 15)) >> 15;
				// Execucao do comportamento
				if(aux == 1)
					i = i | (0b1111111111111111 << 16);
				int64_t subi = (int64_t)R[x] - (int64_t)i;
				R[z] = (uint32_t)subi;
				// Atualizando SR
				R[31] = SR(R[31], ((subi >> 32) & 0b1), 0, 0, ((((R[x] & (0b1 << 31)) >> 31)!=((i & (0b1 << 15)) >> 15)) && (((R[z] & (0b1 << 31)) >> 31)!=((R[x] & (0b1 << 31)) >> 31))), 0, ((R[z] & (0b1 << 31)) >> 31), 0, (R[z]==0), 89);
				idRegistrador(reg1, (uint32_t)z);
				idRegistrador(reg2, (uint32_t)x);
				sprintf(instrucao, "subi %s,%s,%d", reg1, reg2, (int32_t)i);
				// Formatação de saida em arquivo
				idRegistradorM(reg1out, (uint32_t)z);
				idRegistradorM(reg2out, (uint32_t)x);
				fprintf(output, "0x%08X:\t%-25s\t%s=%s-0x%08X=0x%08X,SR=0x%08X\n", R[29], instrucao, reg1out, reg2out, i, R[z], R[31]);
				break;
			//muli
			case 0b010100:
				// Obtendo operandos
				i = R[28] & 0b1111111111111111;
				x = (R[28] & (0b11111 << 16)) >> 16;
				z = (R[28] & (0b11111 << 21)) >> 21;
				aux = (R[28] & (0b1 << 15)) >> 15;
				// Execucao do comportamento
				if(aux == 1)
					i = i | (0b1111111111111111 << 16);
				int64_t muli = (int32_t)R[x] * (int32_t)i;
				R[z] = (uint32_t)muli;
				// Atualizando SR
				R[31] = SR(R[31], 0, 0, 0, 0, (muli>>32), 0, 0, (R[z]==0), 72);
				// Formatacao da instrucao
				idRegistrador(reg1, (uint32_t)z);
				idRegistrador(reg2, (uint32_t)x);
				sprintf(instrucao, "muli %s,%s,%d", reg1, reg2, (int32_t)i);
				// Formatação de saida em arquivo
				idRegistradorM(reg1out, (uint32_t)z);
				idRegistradorM(reg2out, (uint32_t)x);
				fprintf(output, "0x%08X:\t%-25s\t%s=%s*0x%08X=0x%08X,SR=0x%08X\n", R[29], instrucao, reg1out, reg2out, i, R[z], R[31]);
				break;
			//divi
			case 0b010101:
				// Obtendo operandos
				i = R[28] & 0b1111111111111111;
				x = (R[28] & (0b11111 << 16)) >> 16;
				z = (R[28] & (0b11111 << 21)) >> 21;
				aux = (R[28] & (0b1 << 15)) >> 15;
				// Execucao do comportamento
				if(i!=0) {	
					if(aux == 1)
						i = i | (0b1111111111111111 << 16);
					int32_t divi = (int32_t)R[x] / (int32_t)i;
					R[z] = (uint32_t)divi;
				}
				// Atualizando SR
				R[31] = SR(R[31], 0, 0, 0, 0, 0, 0, i==0, R[z]==0, 104);
				// Formatacao da instrucao
				idRegistrador(reg1, (uint32_t)z);
				idRegistrador(reg2, (uint32_t)x);
				sprintf(instrucao, "divi %s,%s,%d", reg1, reg2, (int32_t)i);
				// Formatação de saida em arquivo
				idRegistradorM(reg1out, (uint32_t)z);
				idRegistradorM(reg2out, (uint32_t)x);
				fprintf(output, "0x%08X:\t%-25s\t%s=%s/0x%08X=0x%08X,SR=0x%08X\n", R[29], instrucao, reg1out, reg2out, i, R[z], R[31]);
				break;
			//modi
			case 0b010110:
				// Obtendo operandos
				i = R[28] & 0b1111111111111111;
				x = (R[28] & (0b11111 << 16)) >> 16;
				z = (R[28] & (0b11111 << 21)) >> 21;
				aux = (R[28] & (0b1 << 15)) >> 15;
				// Execucao do comportamento
				if(i!=0) {	
					if(aux == 1)
						i = i | (0b1111111111111111 << 16);
					int32_t modi = (int32_t)R[x] % (int32_t)i;
					R[z] = (uint32_t)modi;
				}
				// Atualizando SR
				R[31] = SR(R[31], 0, 0, 0, 0, 0, 0, (i==0), (R[z]==0), 104);
				// Formatação da instrução
				idRegistrador(reg1, (uint32_t)z);
				idRegistrador(reg2, (uint32_t)x);
				sprintf(instrucao, "modi %s,%s,%d", reg1, reg2, (int32_t)i);
				// Formatação de saida em arquivo
				idRegistradorM(reg1out, (uint32_t)z);
				idRegistradorM(reg2out, (uint32_t)x);
				fprintf(output, "0x%08X:\t%-25s\t%s=%s%%0x%08X=0x%08X,SR=0x%08X\n", R[29], instrucao, reg1out, reg2out, i, R[z], R[31]);
				break;
			//cmpi
			case 0b010111:
				// Obtendo operandos
				i = R[28] & 0b1111111111111111;
				x = (R[28] & (0b11111 << 16)) >> 16;
				aux = (R[28] & (0b1 << 15)) >> 15;
				// Execucao do comportamento
				if(aux==1)
					i = i | (0b1111111111111111 << 16);
				int64_t cmpi = (int64_t)R[x] - (int64_t)i;
				// Atualizando SR
				R[31] = SR(R[31], ((cmpi >> 32) & 0b1), 0, 0, ((((R[x] & (0b1 << 31)) >> 31)!=((i & (0b1 << 15)) >> 15))&&((((cmpi & (0b1 << 31)) >> 31) & 0b1)!=((R[x] & (0b1 << 31)) >> 31))), 0, (((cmpi & (0b1 << 31)) >> 31) & 0b1), 0, (cmpi==0), 89);
				// Formatacao da instrucao
				idRegistrador(reg1, (uint32_t)x);
				sprintf(instrucao, "cmpi %s,%d", reg1, (int32_t)i);
				// Formatacao de saida em arquivo
				fprintf(output, "0x%08X:\t%-25s\tSR=0x%08X\n", R[29], instrucao, R[31]);
				break;
			//l8
			case 0b011000:
				// Otendo operandos
				z = (R[28] & (0b11111 << 21)) >> 21;
				x = (R[28] & (0b11111 << 16)) >> 16;
				i = R[28] & 0xFFFF;
				aux = (R[28] & (0b1 << 15)) >> 15;
				// Execucao do comportamento
				if(aux==1)
					i = i | (0b1111111111111111 << 16);
				R[z] = MEM8[R[x] + i];
				// Formatacao da instrucao
				idRegistrador(reg1, (uint32_t)z);
				idRegistrador(reg2, (uint32_t)x);
				sprintf(instrucao, "l8 %s,[%s%s%i]", reg1, reg2, (z >= 0) ? ("+") : (""), i);
				// Formatacao de saida em arquivo
				idRegistradorM(reg1out, (uint32_t)z);
				fprintf(output, "0x%08X:\t%-25s\t%s=MEM[0x%08X]=0x%02X\n", R[29], instrucao, reg1out, R[x] + i, R[z]);
				break;
			//l16
			case 0b011001:
				// Otendo operandos
				z = (R[28] & (0b11111 << 21)) >> 21;
				x = (R[28] & (0b11111 << 16)) >> 16;
				i = R[28] & 0xFFFF;
				aux = (R[28] & (0b1 << 15)) >> 15;
				// Execucao do comportamento
				if(aux==1)
					i = i | (0b1111111111111111 << 16);
				R[z] = (MEM8[((R[x] + i) << 1)] << 8) | (MEM8[((R[x] + i) << 1) + 1]);
				// Formatacao da instrucao
				idRegistrador(reg1, (uint32_t)z);
				idRegistrador(reg2, (uint32_t)x);
				sprintf(instrucao, "l16 %s,[%s%s%i]", reg1, reg2, (z >= 0) ? ("+") : (""), i);
				// Formatacao de saida em arquivo
				idRegistradorM(reg1out, (uint32_t)z);
				fprintf(output, "0x%08X:\t%-25s\t%s=MEM[0x%08X]=0x%04X\n", R[29], instrucao, reg1out, (R[x] + i) << 1, R[z]);
				break;
			//l32
			case 0b011010:
				// Otendo operandos
				z = (R[28] & (0b11111 << 21)) >> 21;
				x = (R[28] & (0b11111 << 16)) >> 16;
				i = R[28] & 0xFFFF;
				aux = (R[28] & (0b1 << 15)) >> 15;
				// Execucao do comportamento
				if(aux==1)
					i = i | (0b1111111111111111 << 16);
				R[z] = ((MEM8[((R[x] + i) << 2) + 0] << 24) | (MEM8[((R[x] + i) << 2) + 1] << 16) | (MEM8[((R[x] + i) << 2) + 2] << 8) | (MEM8[((R[x] + i) << 2) + 3]));
				idRegistrador(reg1, (uint32_t)z);
				idRegistrador(reg2, (uint32_t)x);
				sprintf(instrucao, "l32 %s,[%s%s%i]", reg1, reg2, (z >= 0) ? ("+") : (""), i);
				// Formatacao de saida em arquivo
				idRegistradorM(reg1out, (uint32_t)z);
				fprintf(output, "0x%08X:\t%-25s\t%s=MEM[0x%08X]=0x%08X\n", R[29], instrucao, reg1out, (R[x] + i) << 2, R[z]);
				break;
			//s8
			case 0b011011:
				// Obtendo operandos
				z = (R[28] & (0b11111 << 21)) >> 21;
				x = (R[28] & (0b11111 << 16)) >> 16;
				i = R[28] & 0xFFFF;
				aux = (R[28] & (0b1 << 15)) >> 15;
				// Execucao do comportamento
				if(aux==1)
					i = i | (0b1111111111111111 << 16);
				MEM8[R[x] + i] = R[z];
				// Formatacao da instrucao
				idRegistrador(reg1, (uint32_t)z);
				idRegistrador(reg2, (uint32_t)x);
				sprintf(instrucao, "s8 [%s%s%i],%s", reg2, (i >= 0) ? ("+") : (""), i, reg1);
				// Formatacao de saida em arquivo
				idRegistradorM(reg1out, (uint32_t)z);
				fprintf(output, "0x%08X:\t%-25s\tMEM[0x%08X]=%s=0x%02X\n", R[29], instrucao, R[x] + i, reg1out, R[z]);	
				break;
			//s16
			case 0b011100:
				// Obtendo operandos
				z = (R[28] & (0b11111 << 21)) >> 21;
				x = (R[28] & (0b11111 << 16)) >> 16;
				i = R[28] & 0xFFFF;
				aux = (R[28] & (0b1 << 15)) >> 15;
				// Execucao do comportamento
				if(aux==1)
					i = i | (0b1111111111111111 << 16);
				MEM8[((R[x] + i) << 1)] = R[z] >> 8;
				MEM8[((R[x] + i) << 1) + 1] = R[z];
				// Formatacao da instrucao
				idRegistrador(reg1, (uint32_t)z);
				idRegistrador(reg2, (uint32_t)x);
				sprintf(instrucao, "s16 [%s%s%i],%s", reg2, (i >= 0) ? ("+") : (""), i, reg1);
				// Formatacao de saida em arquivo
				idRegistradorM(reg1out, (uint32_t)z);
				fprintf(output, "0x%08X:\t%-25s\tMEM[0x%08X]=%s=0x%04X\n", R[29], instrucao, (R[x] + i) << 1, reg1out, R[z]);
				break;
			//s32 
			case 0b011101:
				// Obtendo operandos
				z = (R[28] & (0b11111 << 21)) >> 21;
				x = (R[28] & (0b11111 << 16)) >> 16;
				i = R[28] & 0xFFFF;
				aux = (R[28] & (0b1 << 15)) >> 15;
				// Execucao do comportamento
				if(aux==1)
					i = i | (0b1111111111111111 << 16);
				MEM8[((R[x] + i) << 2)] = R[z] >> 24;
				MEM8[((R[x] + i) << 2) + 1] = R[z] >> 16;
				MEM8[((R[x] + i) << 2) + 2] = R[z] >> 8;
				MEM8[((R[x] + i) << 2) + 3] = R[z];
				// Formatacao da instrucao
				idRegistrador(reg1, (uint32_t)z);
				idRegistrador(reg2, (uint32_t)x);
				sprintf(instrucao, "s32 [%s%s%i],%s", reg2, (i >= 0) ? ("+") : (""), i, reg1);
				// Formatacao de saida em arquivo
				idRegistradorM(reg1out, (uint32_t)z);
				fprintf(output, "0x%08X:\t%-25s\tMEM[0x%08X]=%s=0x%08X\n", R[29], instrucao, (R[x] + i) << 2, reg1out, R[z]);
				break;
			//bae
			case 0b101010:
				// Armazenando o PC antigo
				pc = R[29];
				i = R[28] & 0x3FFFFFF;
				aux = (R[28] & (0b1 << 25)) >> 25;
				// Execucao do comportamento
				if(aux==1)
					i = i | (0b111111 << 26);
				if((R[31] & (0b1))==0)
					R[29] = R[29] + ((int32_t)i << 2);
				// Formatacao da instrucao
				sprintf(instrucao, "bae %d", (int32_t)i);
				// Formatacao de saida em arquivo
				fprintf(output, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao, R[29] + 4);
				break;
			//bat
			case 0b101011:
				// Armazenando o PC antigo
				pc = R[29];
				i = R[28] & 0x3FFFFFF;
				aux = (R[28] & (0b1 << 25)) >> 25;
				// Execucao do comportamento
				if(aux==1)
					i = i | (0b111111 << 26);
				if(((R[31] & (0b1 << 6))==0) && ((R[31] & (0b1))==0))
					R[29] = R[29] + ((int32_t)i << 2);
				// Formatacao da instrucao
				sprintf(instrucao, "bat %d", (int32_t)i);
				// Formatacao de saida em arquivo
				fprintf(output, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao, R[29] + 4);
				break;
			//bbe
			case 0b101100:
				// Armazenando o PC antigo
				pc = R[29];
				aux = (R[31] & (0b1 << 6)) >> 6;
				aux2 = R[31] & 0b1;
				i = R[28] & 0x3FFFFFF;
				uint8_t a = (R[28] & (0b1 << 25)) >> 25;
				// Execucao do comportamento
				if(a==1)
					i = i | (0b111111 << 26);
				if((aux==1) || (aux2==1))
					R[29] = R[29] + ((int32_t)i << 2);
				// Formatacao da instrucao
				sprintf(instrucao, "bbe %d", (int32_t)i);
				// Formatacao de saida em arquivo
				fprintf(output, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao, R[29] + 4);
				break;
			//bbt
			case 0b101101:
				// Armazenando o PC antigo
				pc = R[29];
				i = R[28] & 0x3FFFFFF;
				aux = (R[28] & (0b1 << 25)) >> 25;
				// Execucao do comportamento
				if(aux==1)
					i = i | (0b111111 << 26);
				if((R[31] & (0b1))==1)
					R[29] = R[29] + ((int32_t)i << 2);
				// Formatacao da instrucao
				sprintf(instrucao, "bbt %d", (int32_t)i);
				// Formatacao de saida em arquivo
				fprintf(output, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao, R[29] + 4);
				break;
			//beq
			case 0b101110:
				// Armazenando o PC antigo
				pc = R[29];
				aux = (R[31] & (0b1 << 6)) >> 6;
				i = R[28] & 0x3FFFFFF;
				aux2 = (R[28] & (0b1 << 25)) >> 25;
				// Execucao do comportamento
				if(aux2==1)
					i = i | (0b111111 << 26);
				if(aux == 1)
					R[29] = R[29] + ((int32_t)i << 2);
				// Formatacao da instrucao
				sprintf(instrucao, "beq %d", (int32_t)i);
				// Formatacao de saida em arquivo
				fprintf(output, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao, R[29] + 4);
				break;
			//bge
			case 0b101111:
				// Armazenando o PC antigo
				pc = R[29];
				i = R[28] & 0x3FFFFFF;
				aux = (R[28] & (0b1 << 25)) >> 25;
				// Execucao do comportamento
				if(aux==1)
					i = i | (0b111111 << 26);
				if((R[31] & (0b1 << 4))==(R[31] & (0b1 << 3)))
					R[29] = R[29] + ((int32_t)i << 2);
				// Formatacao da instrucao
				sprintf(instrucao, "bge %d", (int32_t)i);
				// Formatacao de saida em arquivo
				fprintf(output, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao, R[29] + 4);
				break;
			//bgt
			case 0b110000:
				// Armazenando o PC antigo
				pc = R[29];
				i = R[28] & 0x3FFFFFF;
				aux = (R[28] & (0b1 << 25)) >> 25;
				// Execucao do comportamento
				if(aux==1)
					i = i | (0b111111 << 26);
				if(((R[31] & (0b1 << 6))==0) && ((R[31] & (0b1 << 4))==(R[31] & (0b1 << 3))))
					R[29] = R[29] + ((int32_t)i << 2);
				// Formatacao da instrucao
				sprintf(instrucao, "bgt %d", (int32_t)i);
				// Formatacao de saida em arquivo
				fprintf(output, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao, R[29] + 4);
				break;
			//biv
			case 0b110001:
				// Armazenando o PC antigo
				pc = R[29];
				i = R[28] & 0x3FFFFFF;
				aux = (R[28] & (0b1 << 25)) >> 25;
				aux2 = (R[31] & (0b1 << 2)) >> 2;
				// Execucao do comportamento
				if(aux == 1)
					i = i | (0b111111 << 26);
				if(aux2 == 1)
					R[29] = R[29] + ((int32_t)i << 2);
				// Formatacao da instrucao
				sprintf(instrucao, "biv %d", (int32_t)i);
				// Formatacao de saida em arquivo
				fprintf(output, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao, R[29] + 4);
				break;
			//ble
			case 0b110010:
				// Armazenando o PC antigo
				pc = R[29];
				i = R[28] & 0x3FFFFFF;
				aux = (R[28] & (0b1 << 25)) >> 25;
				// Execucao do comportamento
				if(aux==1)
					i = i | (0b111111 << 26);
				if((((R[31] & (0b1 << 6)) >> 6)==1) || (((R[31] & (0b1 << 4)) >> 4)!=((R[31] & (0b1 << 3)) >> 3)))
					R[29] = R[29] + ((int32_t)i << 2);
				// Formatacao da instrucao
				sprintf(instrucao, "ble %d", (int32_t)i);
				// Formatacao de saida em arquivo
				fprintf(output, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao, R[29] + 4);
				break;
			//blt
			case 0b110011:
				// Armazenando o PC antigo
				pc = R[29];
				i = R[28] & 0x3FFFFFF;
				aux = (R[28] & (0b1 << 25)) >> 25;
				// Execucao do comportamento
				if(aux==1)
					i = i | (0b111111 << 26);
				if(((R[31] & (0b1 << 4))!=(R[31] & (0b1 << 3))))
					R[29] = R[29] + ((int32_t)i << 2);
				// Formatacao da instrucao
				sprintf(instrucao, "blt %d", (int32_t)i);
				// Formatacao de saida em arquivo
				fprintf(output, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao, R[29] + 4);
				break;
			//bne
			case 0b110100:
				// Armazenando o PC antigo
				pc = R[29];
				i = R[28] & 0x3FFFFFF;
				aux = (R[28] & (0b1 << 25)) >> 25;
				// Execucao do comportamento
				if(aux==1)
					i = i | (0b111111 << 26);
				if((R[31] & (0b1 << 6))==0)
					R[29] = R[29] + ((int32_t)i << 2);
				// Formatacao da instrucao
				sprintf(instrucao, "bne %d", (int32_t)i);
				// Formatacao de saida em arquivo
				fprintf(output, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao, R[29] + 4);
				break;
			//bni
			case 0b110101:
				// Armazenando o PC antigo
				pc = R[29];
				i = R[28] & 0x3FFFFFF;
				aux = (R[28] & (0b1 << 25)) >> 25;
				// Execucao do comportamento
				if(aux==1)
					i = i | (0b111111 << 26);
				if((R[31] & (0b1 << 2))==0)
					R[29] = R[29] + ((int32_t)i << 2);
				// Formatacao da instrucao
				sprintf(instrucao, "bni %d", (int32_t)i);
				// Formatacao de saida em arquivo
				fprintf(output, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao, R[29] + 4);
				break;
			//bnz
			case 0b110110:
				// Armazenando o PC antigo
				pc = R[29];
				i = R[28] & 0x3FFFFFF;
				aux = (R[28] & (0b1 << 25)) >> 25;
				// Execucao do comportamento
				if(aux==1)
					i = i | (0b111111 << 26);
				if((R[31] & (0b1 << 5))==0)
					R[29] = R[29] + ((int32_t)i << 2);
				// Formatacao da instrucao
				sprintf(instrucao, "bnz %d", (int32_t)i);
				// Formatacao de saida em arquivo
				fprintf(output, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao, R[29] + 4);
				break;
			//bun
			case 0b110111:
				// Armazenando o PC antigo
				pc = R[29];
				i = R[28] & 0x3FFFFFF;
				aux = (R[28] & (0b1 << 25)) >> 25;
				// Execucao do comportamento
				if(aux==1)
					i = i | (0b111111 << 26);
				R[29] = R[29] + ((int32_t)i << 2);
				// Formatacao da instrucao
				sprintf(instrucao, "bun %i", (int32_t)i);
				// Formatacao de saida em arquivo
				fprintf(output, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao, R[29] + 4);
				break;
			//bzd
			case 0b111000:
				// Armazenando o PC antigo
				pc = R[29];
				i = R[28] & 0x3FFFFFF;
				aux = (R[28] & (0b1 << 25)) >> 25;
				aux2 = (R[31] & (0b1 << 5)) >> 5;
				// Execucao do comportamento
				if(aux == 1)
					i = i | (0b111111 << 26);
				if(aux2 == 1)
					R[29] = R[29] + ((int32_t)i << 2);
				// Formatacao da instrucao
				sprintf(instrucao, "bzd %d", (int32_t)i);
				// Formatacao de saida em arquivo
				fprintf(output, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao, R[29] + 4);
				break;
			//call
			case 0b011110:
				// Obtendo operandos
				pc = R[29];
				sp = R[30];
				i = R[28] & 0b1111111111111111;
				x = (R[28] & (0b11111 << 16)) >> 16;
				aux = (R[28] & (0b1 << 15)) >> 15;
				// Execucao do comportamento
				if(aux==1)	
					i = i | (0b1111111111111111 << 16);
				MEM8[sp] = pc >> 24;
				MEM8[sp + 1] = pc >> 16;
				MEM8[sp + 2] = pc >> 8;
				MEM8[sp + 3] = pc;
				R[30] -= 4;
				R[29] = (R[x] + (int32_t)i) << 2;
				R[29] -= 4;
				// Formatacao da instrucao
				idRegistrador(reg1, (uint32_t)x);
				sprintf(instrucao, "call [%s+%d]", reg1, (int32_t)i);
				// Formatacao de saida em arquivo
				fprintf(output, "0x%08X:\t%-25s\tPC=0x%08X,MEM[0x%08X]=0x%08X\n", pc, instrucao, R[29] + 4, sp, pc + 4);
				break;
			//call
			case 0b111001:
				// Obtendo operandos
				pc = R[29];
				sp = R[30];
				i = R[28] & 0x3FFFFFF;
				aux = (R[28] & (0b1 << 26)) >> 26;
				// Execucao do comportamento
				if(aux==1)
					i = i | (0b111111 << 26);
				MEM8[sp] = pc >> 24;
				MEM8[sp + 1] = pc >> 16;
				MEM8[sp + 2] = pc >> 8;
				MEM8[sp + 3] = pc;
				R[30] = R[30] - 4;
				R[29] = R[29] + ((int32_t)i << 2);
				// Formatacao da instrucao
				sprintf(instrucao, "call %d", (int32_t)i);
				// Formatacao de saida em arquivo
				fprintf(output, "0x%08X:\t%-25s\tPC=0x%08X,MEM[0x%08X]=0x%08X\n", pc, instrucao, R[29] + 4, sp, pc + 4);
				break;
			//ret
			case 0b011111:
				// Obtendo operandos
				pc = R[29];
				// Execucao do comportamento
				R[30] += 4;
				sp = R[30];
				R[29] = ((MEM8[sp] << 24) | (MEM8[sp + 1] << 16) | (MEM8[sp + 2] << 8) | ((MEM8[sp + 3])));
				// Formatacao da instrucao
				sprintf(instrucao, "ret");
				// Formatacao de saida em arquivo
				fprintf(output, "0x%08X:\t%-25s\tPC=MEM[0x%08X]=0x%08X\n", pc, instrucao, R[30], R[29] + 4);
				break;
			//push
			case 0b001010:
				// Obtendo operandos
				w = R[28] & 0b11111;
				v = (R[28] & (0b11111 << 6)) >> 6;
				y = (R[28] & (0b11111 << 11)) >> 11;
				x = (R[28] & (0b11111 << 16)) >> 16;
				z = (R[28] & (0b11111 << 21)) >> 21;
				sp = R[30];
				// Execucao do comportamento
				uint8_t push = 0;
				int h = 0;
				if(v!=0) {
					push = v;
					MEM8[R[30]] = R[push] >> 24;
					MEM8[R[30] + 1] = R[push] >> 16;
					MEM8[R[30] + 2] = R[push] >> 8;
					MEM8[R[30] + 3] = R[push];
					R[30] = sp - 4;
					h = 1;
					if(w!=0) {
						push = w;
						MEM8[R[30]] = R[push] >> 24;
						MEM8[R[30] + 1] = R[push] >> 16;
						MEM8[R[30] + 2] = R[push] >> 8;
						MEM8[R[30] + 3] = R[push];
						R[30] = sp - 8;
						h = 2;
						if(x!=0) {
							push = x;
							MEM8[R[30]] = R[push] >> 24;
							MEM8[R[30] + 1] = R[push] >> 16;
							MEM8[R[30] + 2] = R[push] >> 8;
							MEM8[R[30] + 3] = R[push];
							R[30] = sp - 12;
							h = 3;
							if(y!=0) {
								push = y;
								MEM8[R[30]] = R[push] >> 24;
								MEM8[R[30] + 1] = R[push] >> 16;
								MEM8[R[30] + 2] = R[push] >> 8;
								MEM8[R[30] + 3] = R[push];
								R[30] = sp -16;
								h = 4;
								if(z!=0) {
									push = z;
									MEM8[R[30]] = R[push] >> 24;
									MEM8[R[30] + 1] = R[push] >> 16;
									MEM8[R[30] + 2] = R[push] >> 8;
									MEM8[R[30] + 3] = R[push];
									R[30] = sp - 20;
									h = 5;
								}
							}
						}
					}
				}
				if(h==0) {
					// Formatacao da instrucao
					sprintf(instrucao, "push -");
					// Formatacao de saida em arquivo
					fprintf(output, "0x%08X:\t%-25s\tMEM[0x%08X]{}={}\n", R[29], instrucao, sp);
				}
				if(h==1) {
					// Formatacao da instrucao
					idRegistrador(reg1, (uint32_t)v);
					sprintf(instrucao, "push %s", reg1);
					// Formatacao de saida em arquivo
					idRegistradorM(reg1out, (uint32_t)v);
					fprintf(output, "0x%08X:\t%-25s\tMEM[0x%08X]{0x%08X}={%s}\n", R[29], instrucao, sp, R[v], reg1out);
				}
				if(h==2) {
					// Formatacao da instrucao
					idRegistrador(reg1, (uint32_t)v);
					idRegistrador(reg2, (uint32_t)w);
					sprintf(instrucao, "push %s,%s", reg1, reg2);
					// Formatacao de saida em arquivo
					idRegistradorM(reg1out, (uint32_t)v);
					idRegistradorM(reg2out, (uint32_t)w);
					fprintf(output, "0x%08X:\t%-25s\tMEM[0x%08X]{0x%08X,0x%08X}={%s,%s}\n", R[29], instrucao, sp, R[v], R[w], reg1out, reg2out);
				}
				if(h==3) {
					// Formatacao da instrucao
					idRegistrador(reg1, (uint32_t)v);
					idRegistrador(reg2, (uint32_t)w);
					idRegistrador(reg3, (uint32_t)x);
					sprintf(instrucao, "push %s,%s,%s", reg1, reg2, reg3);
					// Formatacao de saida em arquivo
					idRegistradorM(reg1out, (uint32_t)v);
					idRegistradorM(reg2out, (uint32_t)w);
					idRegistradorM(reg3out, (uint32_t)x);
					fprintf(output, "0x%08X:\t%-25s\tMEM[0x%08X]{0x%08X,0x%08X,0x%08X}={%s,%s,%s}\n", R[29], instrucao, sp, R[v], R[w], R[x], reg1out, reg2out, reg3out);
				}
				if(h==4) {
					// Formatacao da instrucao
					idRegistrador(reg1, (uint32_t)v);
					idRegistrador(reg2, (uint32_t)w);
					idRegistrador(reg3, (uint32_t)x);
					idRegistrador(reg4, (uint32_t)y);
					sprintf(instrucao, "push %s,%s,%s,%s", reg1, reg2, reg3, reg4);
					// Formatacao de saida em arquivo
					idRegistradorM(reg1out, (uint32_t)v);
					idRegistradorM(reg2out, (uint32_t)w);
					idRegistradorM(reg3out, (uint32_t)x);
					idRegistradorM(reg4out, (uint32_t)y);
					fprintf(output, "0x%08X:\t%-25s\tMEM[0x%08X]{0x%08X,0x%08X,0x%08X,0x%08X}={%s,%s,%s,%s}\n", R[29], instrucao, sp, R[v], R[w], R[x], R[y], reg1out, reg2out, reg3out, reg4out);
				}
				if(h==5) {
					// Formatacao da instrucao
					idRegistrador(reg1, (uint32_t)v);
					idRegistrador(reg2, (uint32_t)w);
					idRegistrador(reg3, (uint32_t)x);
					idRegistrador(reg4, (uint32_t)y);
					idRegistrador(reg5, (uint32_t)z);
					sprintf(instrucao, "push %s,%s,%s,%s,%s", reg1, reg2, reg3, reg4, reg5);
					// Formatacao de saida em arquivo
					idRegistradorM(reg1out, (uint32_t)v);
					idRegistradorM(reg2out, (uint32_t)w);
					idRegistradorM(reg3out, (uint32_t)x);
					idRegistradorM(reg4out, (uint32_t)y);
					idRegistradorM(reg5out, (uint32_t)z);
					fprintf(output, "0x%08X:\t%-25s\tMEM[0x%08X]{0x%08X,0x%08X,0x%08X,0x%08X,0x%08X}={%s,%s,%s,%s,%s}\n", R[29], instrucao, sp, R[v], R[w], R[x], R[y], R[z], reg1out, reg2out, reg3out, reg4out, reg5out);
				}
				break;
			//pop
			case 0b001011:
				// Obtendo operandos
				w = R[28] & 0b11111;
				v = (R[28] & (0b11111 << 6)) >> 6;
				y = (R[28] & (0b11111 << 11)) >> 11;
				x = (R[28] & (0b11111 << 16)) >> 16;
				z = (R[28] & (0b11111 << 21)) >> 21;
				sp = R[30];
				// Execucao do comportamento
				uint8_t pop = 0;
				int k = 0;
				if(v!=0) {
					pop = v;
					R[30] = sp + 4;
					R[pop] = ((MEM8[R[30]] << 24) | (MEM8[R[30] + 1] << 16) | (MEM8[R[30] + 2] << 8) | ((MEM8[R[30] + 3])));
					k = 1;
					if(w!=0) {
						pop = w;
						R[30] = sp + 8;
						R[pop] = ((MEM8[R[30]] << 24) | (MEM8[R[30] + 1] << 16) | (MEM8[R[30] + 2] << 8) | ((MEM8[R[30] + 3])));
						k = 2;
						if(x!=0) {
							pop = x;
							R[30] = sp + 12;
							R[pop] = ((MEM8[R[30]] << 24) | (MEM8[R[30] + 1] << 16) | (MEM8[R[30] + 2] << 8) | ((MEM8[R[30] + 3])));
							k = 3;
							if(y!=0) {
								pop = y;
								R[30] = sp + 16;
								R[pop] = ((MEM8[R[30]] << 24) | (MEM8[R[30] + 1] << 16) | (MEM8[R[30] + 2] << 8) | ((MEM8[R[30] + 3])));
								k = 4;
								if(z!=0) {
									pop = z;
									R[30] = sp + 20;
									R[pop] = ((MEM8[R[30]] << 24) | (MEM8[R[30] + 1] << 16) | (MEM8[R[30] + 2] << 8) | ((MEM8[R[30] + 3])));
									k = 5;
								}
							}
						}
					}
				}	
				if(k==0) {
					// Formatacao da instrucao
					sprintf(instrucao, "pop -");
					// Formatacao de saida em arquivo
					fprintf(output, "0x%08X:\t%-25s\t{}=MEM[0x%08X]{}\n", R[29], instrucao, sp);
				}
				if(k==1) {
					// Formatacao da instrucao
					idRegistrador(reg1, (uint32_t)v);
					sprintf(instrucao, "pop %s", reg1);
					// Formatacao de saida em arquivo
					idRegistradorM(reg1out, (uint32_t)v);
					fprintf(output, "0x%08X:\t%-25s\t{%s}=MEM[0x%08X]{0x%08X}\n", R[29], instrucao, reg1out, sp, R[v]);
				}
				if(k==2) {
					// Formatacao da instrucao
					idRegistrador(reg1, (uint32_t)v);
					idRegistrador(reg2, (uint32_t)w);
					sprintf(instrucao, "pop %s,%s", reg1, reg2);
					// Formatacao de saida em arquivo
					idRegistradorM(reg1out, (uint32_t)v);
					idRegistradorM(reg2out, (uint32_t)w);
					fprintf(output, "0x%08X:\t%-25s\t{%s,%s}=MEM[0x%08X]{0x%08X,0x%08X}\n", R[29], instrucao, reg1out, reg2out, sp, R[v], R[w]);
				}
				if(k==3) {
					// Formatacao da instrucao
					idRegistrador(reg1, (uint32_t)v);
					idRegistrador(reg2, (uint32_t)w);
					idRegistrador(reg3, (uint32_t)x);
					sprintf(instrucao, "pop %s,%s,%s", reg1, reg2, reg3);
					// Formatacao de saida em arquivo
					idRegistradorM(reg1out, (uint32_t)v);
					idRegistradorM(reg2out, (uint32_t)w);
					idRegistradorM(reg3out, (uint32_t)x);
					fprintf(output, "0x%08X:\t%-25s\t{%s,%s,%s}=MEM[0x%08X]{0x%08X,0x%08X,0x%08X}\n", R[29], instrucao, reg1out, reg2out, reg3out, sp, R[v], R[w], R[x]);
				}
				if(k==4) {
					// Formatacao da instrucao
					idRegistrador(reg1, (uint32_t)v);
					idRegistrador(reg2, (uint32_t)w);
					idRegistrador(reg3, (uint32_t)x);
					idRegistrador(reg4, (uint32_t)y);
					sprintf(instrucao, "pop %s,%s,%s,%s", reg1, reg2, reg3, reg4);
					// Formatacao de saida em arquivo
					idRegistradorM(reg1out, (uint32_t)v);
					idRegistradorM(reg2out, (uint32_t)w);
					idRegistradorM(reg3out, (uint32_t)x);
					idRegistradorM(reg4out, (uint32_t)y);
					fprintf(output, "0x%08X:\t%-25s\t{%s,%s,%s,%s}=MEM[0x%08X]{0x%08X,0x%08X,0x%08X,0x%08X}\n", R[29], instrucao, reg1out, reg2out, reg3out, reg4out, sp, R[v], R[w], R[x], R[y]);
				}
				if(k==5) {
					// Formatacao da instrucao
					idRegistrador(reg1, (uint32_t)v);
					idRegistrador(reg2, (uint32_t)w);
					idRegistrador(reg3, (uint32_t)x);
					idRegistrador(reg4, (uint32_t)y);
					idRegistrador(reg5, (uint32_t)z);
					sprintf(instrucao, "pop %s,%s,%s,%s,%s", reg1, reg2, reg3, reg4, reg5);
					// Formatacao de saida em arquivo
					idRegistradorM(reg1out, (uint32_t)v);
					idRegistradorM(reg2out, (uint32_t)w);
					idRegistradorM(reg3out, (uint32_t)x);
					idRegistradorM(reg4out, (uint32_t)y);
					idRegistradorM(reg5out, (uint32_t)z);
					fprintf(output, "0x%08X:\t%-25s\t{%s,%s,%s,%s,%s}=MEM[0x%08X]{0x%08X,0x%08X,0x%08X,0x%08X,0x%08X}\n", R[29], instrucao, reg1out, reg2out, reg3out, reg4out, reg5out, sp, R[v], R[w], R[x], R[y], R[z]);
				}
				break;
			//int
			case 0b111111:
				// Parar a execucao
				executa = 0;
				// Formatacao da instrucao
				sprintf(instrucao, "int 0");
				// Formatacao de saida em tela
				fprintf(output, "0x%08X:\t%-25s\tCR=0x00000000,PC=0x00000000\n", R[29], instrucao);
				break;
			//Instrucao desconhecida
			default:
				// Exibindo mensagem de erro
				fprintf(output, "Instrucao desconhecida!\n");
				// Parar a execucao
				executa = 0;
		}
		// PC = PC + 4 (proxima instrucao)
		R[29] = R[29] + 4;
	}
	// Exibindo a finalizacao da execucao
	fprintf(output, "[END OF SIMULATION]");
	// Fechando os arquivos
	fclose(input);
	fclose(output);
	// Finalizando programa
	return 0;
}