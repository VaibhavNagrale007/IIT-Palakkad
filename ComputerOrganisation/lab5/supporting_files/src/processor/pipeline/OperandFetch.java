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
    
    public static boolean conflict(Instruction instruction, int r1, int r2) {		// it checks for conflicts
		int inst_ordinal = 0,dest_reg=0;
		if (instruction != null && instruction.getOperationType() != null){
			inst_ordinal = instruction.getOperationType().ordinal();
		}else{
			return false;
		}
		if (inst_ordinal <= 23) {
			if (instruction != null){
				dest_reg = instruction.getDestinationOperand().getValue();
			}else{
				dest_reg = -1;
			}
			if (r1 == dest_reg || r2 == dest_reg) {
				return true;
			}else{
				return false;
			}
		}
		return false;
	}
	
	public boolean division_conflict(int r1, int r2) {
		Instruction instruction_ex_stage = OF_EX_Latch.getInstruction();
		Instruction instruction_ma_stage = EX_MA_Latch.getInstruction();
		Instruction instruction_rw_stage = MA_RW_Latch.getInstruction();
		if (r1 == 31 || r2 == 31) {
		    int inst_ex_ordinal=0, inst_ma_ordinal=0, inst_rw_ordinal=0;
		    // first checking for null instruction
			if (instruction_ex_stage != null && instruction_ex_stage.getOperationType() != null){
				inst_ex_ordinal = instruction_ex_stage.getOperationType().ordinal();
			}else{
				inst_ex_ordinal = -1;
			}
			
			if (instruction_ma_stage != null && instruction_ma_stage.getOperationType() != null){
				inst_ma_ordinal = instruction_ma_stage.getOperationType().ordinal();
			}else{
				inst_ma_ordinal = -1;
			}
			
			if (instruction_rw_stage != null && instruction_rw_stage.getOperationType() != null){
				inst_rw_ordinal = instruction_rw_stage.getOperationType().ordinal();
			}else{
				inst_rw_ordinal = -1;
			}
			
			if (inst_ex_ordinal == 6 || inst_ex_ordinal == 7 || inst_ma_ordinal == 6 || inst_ma_ordinal == 7 || inst_rw_ordinal == 6 || inst_rw_ordinal == 7) {
				return true;
			}
			return false;
		}
		return false;
	}
	
	public void bubble_conflict () {		// Conflict is present 
		IF_EnableLatch.setIF_enable(false);
		OF_EX_Latch.setIsNOP(true);
	}

	public void performOF() {
		// checking for busy
		if (IF_OF_Latch.OF_busy){
			return;
		}
		if (IF_EnableLatch.isIF_busy()){
			OF_EX_Latch.setIsNOP(true);
			return;
		}
		if (IF_OF_Latch.isOF_enable()) {
			// if OF is enabled then fetch the instructions
			OperationType[] operationType = OperationType.values();
			int instruction = IF_OF_Latch.getInstruction();
			
			int imm=0,imm_val=0;
			int opcode = extract_inst(instruction,0,27);
			OperationType operation = operationType[opcode];
			
			if (operation.ordinal() >= 24 && operation.ordinal() <= 28 ) {
				IF_EnableLatch.setIF_enable(false);
			}
			
			boolean conflict_inst = false;
			Instruction instruction_ex_stage = OF_EX_Latch.getInstruction();
			Instruction instruction_ma_stage = EX_MA_Latch.getInstruction();
			Instruction instruction_rw_stage = MA_RW_Latch.getInstruction();
			Instruction inst = new Instruction();
			switch (operation) {
			case add:	// R3 Type
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
				// checking for conflicts before fetching rd
				if (conflict(instruction_ex_stage, registerNo, registerNo2))
				{
					conflict_inst = true;
				}
				if (conflict(instruction_ma_stage, registerNo, registerNo2))
				{
					conflict_inst = true;
				}
				if (conflict(instruction_rw_stage, registerNo, registerNo2))
				{
					conflict_inst = true;
				}
				if (division_conflict(registerNo, registerNo2))
				{
					conflict_inst = true;
				}
				if (conflict_inst) {
					this.bubble_conflict();
					break;
				}
				

				Operand rd = new Operand();
				rd.setOperandType(OperandType.Register);
				registerNo = extract_inst(instruction,15,27);
				rd.setValue(registerNo);

				inst.setOperationType(operationType[opcode]);
				inst.setSourceOperand1(rs1);
				inst.setSourceOperand2(rs2);
				inst.setDestinationOperand(rd);
				break;

			case addi:	// R2 Type
			case subi:
			case muli:
			case divi:
			case andi:
			case ori:
			case xori:
			case slti:
			case slli:
			case srli:
			case srai:
			case load:
			case store:
				rs1 = new Operand();// rs1
				rs1.setOperandType(OperandType.Register);
				registerNo = extract_inst(instruction,5,27);
				rs1.setValue(registerNo);
				// checking for conflicts before fetching rd
				if (conflict(instruction_ex_stage, registerNo, registerNo))
				{
					conflict_inst = true;
				}	
				if (conflict(instruction_ma_stage, registerNo, registerNo))
				{
					conflict_inst = true;
				}
				if (conflict(instruction_rw_stage, registerNo, registerNo))
				{
					conflict_inst = true;
				}
				if (division_conflict(registerNo, registerNo))
				{
					conflict_inst = true;
				}	
				if (conflict_inst) {
					this.bubble_conflict();
					break;
				}

				rd = new Operand();// rd
				rd.setOperandType(OperandType.Register);
				registerNo = extract_inst(instruction,10,27);
				rd.setValue(registerNo);

				rs2 = new Operand();// imm
				rs2.setOperandType(OperandType.Immediate);
				imm = extract_inst_imm(instruction,15,15);
				imm_val = imm;
				rs2.setValue(imm_val);
				inst.setOperationType(operationType[opcode]);
				inst.setSourceOperand1(rs1);
				inst.setSourceOperand2(rs2);
				inst.setDestinationOperand(rd);

				break;
				
			case jmp:	// R1 Type
				Operand op = new Operand();
				imm = extract_inst_imm(instruction,10,10);
				imm_val = imm;
				if (imm_val != 0) {
					op.setOperandType(OperandType.Immediate);
					op.setValue(imm_val);
				} else {
					registerNo = extract_inst(instruction,5,27);
					op.setOperandType(OperandType.Register);
					op.setValue(registerNo);
				}

				inst.setOperationType(operationType[opcode]);
				inst.setDestinationOperand(op);
				break;

			case beq:	// R2 Type
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
				// checking for conflicts before fetching rd
				if (conflict(instruction_ex_stage, registerNo, registerNo2))
				{
					conflict_inst = true;
				}
				if (conflict(instruction_ma_stage, registerNo, registerNo2))
				{
					conflict_inst = true;
				}
				if (conflict(instruction_rw_stage, registerNo, registerNo2))
				{
					conflict_inst = true;
				}
				if (division_conflict(registerNo, registerNo2))
				{
					conflict_inst = true;
				}
				if (conflict_inst) {
					this.bubble_conflict();
					break;
				}
				
				rd = new Operand();// rd --> imm
				rd.setOperandType(OperandType.Immediate);
				imm = extract_inst_imm(instruction,15,15);
				imm_val = imm;
				rd.setValue(imm_val);
				
				inst.setOperationType(operationType[opcode]);
				inst.setSourceOperand1(rs1);
				inst.setSourceOperand2(rs2);
				inst.setDestinationOperand(rd);
				break;

			case end:	// R1 Type
				inst.setOperationType(operationType[opcode]);
				IF_EnableLatch.setIF_enable(false);
				break;
			
			}// setting boolena values at end of Fetch
			if (!OF_EX_Latch.getIsNOP()){
				IF_OF_Latch.setOF_enable(false);
			}
			OF_EX_Latch.setInstruction(inst);
			OF_EX_Latch.setEX_enable(true);
			
		}
	}

}
