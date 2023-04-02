package processor.pipeline;

import generic.Instruction;

public class EX_MA_LatchType {
	
	boolean MA_enable;
	int alu_result;
	boolean NOP;
	Instruction instruction;
	boolean busy_MA;

	public EX_MA_LatchType()
	{
		MA_enable = false;
		NOP = false;
		busy_MA = false;
	}

	public boolean isMA_enable() {
		return MA_enable;
	}

	public void setMA_enable(boolean mA_enable) {
		MA_enable = mA_enable;
	}

	public Instruction getInstruction() {
		return instruction;
	}

	public void setInstruction(Instruction inst) {
		instruction = inst;
	}

	public int getALU_result() {
		return alu_result;
	}

	public void setALU_result(int result) {
		alu_result = result;
	}
	
	// NOP instruction for bubble in pipeline
	public boolean getIsNOP() {
		return NOP;
	}
	public void setIsNOP(boolean is_NOP) {
		NOP = is_NOP;
	}
	
	// setting and retuning boolean value whether MA is busy or not
	public void setMA_busy(boolean bool_ma) {
		busy_MA = bool_ma;
	}
	public boolean isMA_busy() {
		return busy_MA;
	}
}
