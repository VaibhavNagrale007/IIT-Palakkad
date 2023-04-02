package processor.pipeline;

import generic.Simulator;
import processor.Clock;
import generic.Element;
import generic.MemoryReadEvent;
import generic.MemoryResponseEvent;
import generic.MemoryWriteEvent;
import generic.Event;
import generic.Event.EventType;
import configuration.Configuration;
import processor.Processor;
import generic.Instruction;
import generic.Instruction.OperationType;

public class MemoryAccess implements Element{
	Processor containingProcessor;
	public Instruction instruction;
	public EX_MA_LatchType EX_MA_Latch;
	public MA_RW_LatchType MA_RW_Latch;
	IF_EnableLatchType IF_EnableLatch;

	public MemoryAccess(Processor containingProcessor, EX_MA_LatchType eX_MA_Latch, MA_RW_LatchType mA_RW_Latch, IF_EnableLatchType iF_EnableLatch) {
		this.containingProcessor = containingProcessor;
		this.EX_MA_Latch = eX_MA_Latch;
		this.MA_RW_Latch = mA_RW_Latch;
		this.IF_EnableLatch = iF_EnableLatch;
	}

	public void performMA() {
		// check for busy
		if (EX_MA_Latch.isMA_busy()){
			containingProcessor.getEXUnit().OF_EX_Latch.EX_busy = true;
			return;
		}else{
			containingProcessor.getEXUnit().OF_EX_Latch.EX_busy = false;
		}
		if (EX_MA_Latch.getIsNOP()) { //if the control signal getIsNop is true means the instruction is Nop
			MA_RW_Latch.setIsNOP(true);//so setting the IsNOP signal to true
			MA_RW_Latch.setInstruction(null);//and instruction  to empty(null)
			EX_MA_Latch.setIsNOP(false);
		} else if (EX_MA_Latch.isMA_enable()) { //if EX_MA latch is enabled 
			instruction = EX_MA_Latch.getInstruction(); //taking instruction from the latch
			int alu_result = EX_MA_Latch.getALU_result(); //also taking the alu result 
			// System.out.println("MA is enabled: " + instruction);
			MA_RW_Latch.setALU_result(alu_result);//setting the alureslt to the next latch
			OperationType opcode = instruction.getOperationType();  //getting the opcode(operation type) from the instruction
			MA_RW_Latch.setInstruction(instruction); //setting the instruction to the next latch
			switch (opcode) {
			case store:    //if the operation is store
				int val_store = containingProcessor.getRegisterFile() //take the value to be stored in the memory from the register file
						.getValue(instruction.getSourceOperand1().getValue());
				// add event (Memory Write Event in store)
				Simulator.getEventQueue().addEvent(
						new MemoryWriteEvent(
							Clock.getCurrentTime() + containingProcessor.getL1dCache().getCacheLatency(),
							this,containingProcessor.getL1dCache(),alu_result,val_store));
					EX_MA_Latch.setMA_busy(true);
					return;
				//break;

			case load:  //if the operation load 
				// add event (Memory Read Event in load)
				Simulator.getEventQueue().addEvent(
						new MemoryReadEvent(
							Clock.getCurrentTime() + containingProcessor.getL1dCache().getCacheLatency(),
							this,containingProcessor.getL1dCache(),alu_result));
					EX_MA_Latch.setMA_busy(true);
					return;
				//break;

			case end:    //if the operation is end 
			    IF_EnableLatch.setIF_enable(false); //setting the IFenable latch to false 
			    break;
			
			default:
				break;
			}
			
			EX_MA_Latch.setMA_enable(false);
			
			MA_RW_Latch.setRW_enable(true);          //enabling the MA_RW latch to true
			// EX_MA_Latch.setMA_enable(false);
		}
	}
	
	public void handleEvent(Event e){
		// for handling MA load event
		if (e.getEventType() == EventType.MemoryResponse){
			MemoryResponseEvent event = (MemoryResponseEvent) e;
			int ld_res = event.getValue();		// load result
			MA_RW_Latch.setLoad_result(ld_res);
			MA_RW_Latch.setInstruction(instruction);
			MA_RW_Latch.setRW_enable(true);
			EX_MA_Latch.setMA_busy(false);
			containingProcessor.getEXUnit().OF_EX_Latch.EX_busy = false;
			EX_MA_Latch.setMA_enable(false);
		}
	}

}
