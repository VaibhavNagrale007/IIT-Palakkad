package processor.pipeline;

import processor.Processor;
import generic.Instruction;
import generic.Instruction.OperationType;
import generic.Operand;
import generic.Statistics;
import generic.Operand.OperandType;

public class OperandFetch {
	Processor containingProcessor;
	IF_OF_LatchType IF_OF_Latch;
	OF_EX_LatchType OF_EX_Latch;
	EX_MA_LatchType EX_MA_Latch;
	MA_RW_LatchType MA_RW_Latch;
	IF_EnableLatchType IF_EnableLatch;

	public OperandFetch(Processor containingProcessor, IF_OF_LatchType iF_OF_Latch, OF_EX_LatchType oF_EX_Latch, EX_MA_LatchType eX_MA_Latch, MA_RW_LatchType mA_RW_Latch, IF_EnableLatchType iF_EnableLatch) {
		this.containingProcessor = containingProcessor;
		this.IF_OF_Latch = iF_OF_Latch;
		this.OF_EX_Latch = oF_EX_Latch;
		this.EX_MA_Latch = eX_MA_Latch;
		this.MA_RW_Latch = mA_RW_Latch;
		this.IF_EnableLatch = iF_EnableLatch;
	}
 	public int extract_inst(int n,int i,int j){
        n = n << i;
        n = n >>> j;
        return n;
    }
    public int extract_inst_imm(int n,int i,int j){
        n = n << i;
        n = n >> j;
        return n;
    }

	public void performOF() {
		if (IF_OF_Latch.isOF_enable()) {
			Statistics.setNumberOfOFInstructions(Statistics.getNumberOfOFInstructions() + 1);
			OperationType[] operationType = OperationType.values();
			int instruction = IF_OF_Latch.getInstruction();
			
			int opcode = extract_inst(instruction,0,27);
			int type_operation = opcode;
			OperationType operation = operationType[type_operation];
			
			if (operation.ordinal() == 24 || operation.ordinal() == 25 || operation.ordinal() == 26 || operation.ordinal() == 27 || operation.ordinal() == 28 ) {
				IF_EnableLatch.setIF_enable(false);
			}
			

			
			boolean conflict_inst = false;
			Instruction instruction_ex_stage = OF_EX_Latch.getInstruction();
			Instruction instruction_ma_stage = EX_MA_Latch.getInstruction();
			Instruction instruction_rw_stage = MA_RW_Latch.getInstruction();
			Instruction inst = new Instruction();
			switch (operation) {
			case add:
			case sub:
			case mul:
			case div:
			case and:
			case or:
			case xor:
			case slt:
			case sll:
			case srl:
			case sra:
				Operand rs1 = new Operand();
				rs1.setOperandType(OperandType.Register);
				int registerNo = extract_inst(instruction,5,27);
				rs1.setValue(registerNo);

				Operand rs2 = new Operand();
				rs2.setOperandType(OperandType.Register);
				int registerNo2 = extract_inst(instruction,10,27);
				rs2.setValue(registerNo2);
				

				Operand rd = new Operand();
				rd.setOperandType(OperandType.Register);
				registerNo = extract_inst(instruction,15,27);
				rd.setValue(registerNo);

				inst.setOperationType(operationType[type_operation]);
				inst.setSourceOperand1(rs1);
				inst.setSourceOperand2(rs2);
				inst.setDestinationOperand(rd);
				break;

			case end:
				inst.setOperationType(operationType[type_operation]);
				IF_EnableLatch.setIF_enable(false);
				break;
			case jmp:
				Operand op = new Operand();
				int imm = extract_inst_imm(instruction,10,10);
				int imm_val = imm;
				if (imm_val != 0) {
					op.setOperandType(OperandType.Immediate);
					op.setValue(imm_val);
				} else {
					registerNo = extract_inst(instruction,5,27);
					op.setOperandType(OperandType.Register);
					op.setValue(registerNo);
				}

				inst.setOperationType(operationType[type_operation]);
				inst.setDestinationOperand(op);
				break;

			case beq:
			case bne:
			case blt:
			case bgt:
				rs1 = new Operand();
				rs1.setOperandType(OperandType.Register);
				registerNo = extract_inst(instruction,5,27);
				rs1.setValue(registerNo);
				
				rs2 = new Operand();// our rs2 --> destination
				rs2.setOperandType(OperandType.Register);
				registerNo2 = extract_inst(instruction,10,27);
				rs2.setValue(registerNo2);
				
				rd = new Operand();// rd --> imm
				rd.setOperandType(OperandType.Immediate);
				imm = extract_inst_imm(instruction,15,15);
				imm_val = imm;
				rd.setValue(imm_val);
				
				inst.setOperationType(operationType[type_operation]);
				inst.setSourceOperand1(rs1);
				inst.setSourceOperand2(rs2);
				inst.setDestinationOperand(rd);
				break;

			default:
				rs1 = new Operand();// rs1
				rs1.setOperandType(OperandType.Register);
				registerNo = extract_inst(instruction,5,27);
				rs1.setValue(registerNo);

				rd = new Operand();// rd
				rd.setOperandType(OperandType.Register);
				registerNo = extract_inst(instruction,10,27);
				rd.setValue(registerNo);

				rs2 = new Operand();// imm
				rs2.setOperandType(OperandType.Immediate);
				imm = extract_inst_imm(instruction,15,15);
				imm_val = imm;
				rs2.setValue(imm_val);
				inst.setOperationType(operationType[type_operation]);
				inst.setSourceOperand1(rs1);
				inst.setSourceOperand2(rs2);
				inst.setDestinationOperand(rd);

				break;
			}
			OF_EX_Latch.setInstruction(inst);
			OF_EX_Latch.setEX_enable(true);
		}
	}

}
