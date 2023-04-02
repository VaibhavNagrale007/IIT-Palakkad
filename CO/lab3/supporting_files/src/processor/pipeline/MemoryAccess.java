package processor.pipeline;

import processor.Processor;
import generic.Instruction;
import generic.Instruction.OperationType;

public class MemoryAccess {
	Processor containingProcessor;
	EX_MA_LatchType EX_MA_Latch;
	MA_RW_LatchType MA_RW_Latch;
	IF_EnableLatchType IF_EnableLatch;

	public MemoryAccess(Processor containingProcessor, EX_MA_LatchType eX_MA_Latch, MA_RW_LatchType mA_RW_Latch, IF_EnableLatchType iF_EnableLatch) {
		this.containingProcessor = containingProcessor;
		this.EX_MA_Latch = eX_MA_Latch;
		this.MA_RW_Latch = mA_RW_Latch;
		this.IF_EnableLatch = iF_EnableLatch;
	}

	public void performMA() {
		if (EX_MA_Latch.getIsNOP()) { //if the control signal getIsNop is true means the instruction is Nop
			MA_RW_Latch.setIsNOP(true);//so setting the IsNOP signal to true
			MA_RW_Latch.setInstruction(null);//and instruction  to empty(null)
			EX_MA_Latch.setIsNOP(false);
		} else if (EX_MA_Latch.isMA_enable()) { //if EX_MA latch is enabled 
			Instruction instruction = EX_MA_Latch.getInstruction(); //taking instruction from the latch
			int alu_result = EX_MA_Latch.getALU_result(); //also taking the alu result 
			// System.out.println("MA is enabled: " + instruction);
			MA_RW_Latch.setALU_result(alu_result);//setting the alureslt to the next latch
			OperationType opcode = instruction.getOperationType();  //getting the opcode(operation type) from the instruction
			switch (opcode) {
			case store:    //if the operation is store
				int val_store = containingProcessor.getRegisterFile() //take the value to be stored in the memory from the register file
						.getValue(instruction.getSourceOperand1().getValue());
				containingProcessor.getMainMemory().setWord(alu_result, val_store);// storing the value in the memory in  memory location alu_result 
				break;

			case load:  //if the operation load 
				int load_result = containingProcessor.getMainMemory().getWord(alu_result);//taking the value present in the memory at the memory loaction alu result
				MA_RW_Latch.setLoad_result(load_result); //setting the loaded value  to the next latch
				break;

			case end:    //if the operation is end 
			    IF_EnableLatch.setIF_enable(false); //setting the IFenable latch to false 
			    break;
			
			default:
				break;
			}
			
			MA_RW_Latch.setInstruction(instruction); //setting the instruction to the next latch
			MA_RW_Latch.setRW_enable(true);          //enabling the MA_RW latch to true
			// EX_MA_Latch.setMA_enable(false);
		}
	}

}
