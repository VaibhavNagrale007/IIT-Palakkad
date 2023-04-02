package processor.pipeline;
import generic.Simulator;
import processor.Processor;
import processor.Clock;
import generic.Statistics;
import generic.Element;
import generic.MemoryReadEvent;
import generic.MemoryResponseEvent;
import generic.Event;
import generic.Event.EventType;
import configuration.Configuration;
import generic.Instruction.OperationType;

public class InstructionFetch implements Element {
	
	Processor containingProcessor;
	IF_EnableLatchType IF_EnableLatch;
	IF_OF_LatchType IF_OF_Latch;
	EX_IF_LatchType EX_IF_Latch;
	
	public InstructionFetch(Processor containingProcessor, IF_EnableLatchType iF_EnableLatch, IF_OF_LatchType iF_OF_Latch, EX_IF_LatchType eX_IF_Latch)
	{
		this.containingProcessor = containingProcessor;
		this.IF_EnableLatch = iF_EnableLatch;
		this.IF_OF_Latch = iF_OF_Latch;
		this.EX_IF_Latch = eX_IF_Latch;
	}
	public void performIF ()
    {
        if( IF_EnableLatch.isIF_enable())
        {
            if( IF_EnableLatch.isIF_busy())
            {
                return ;
            }
            if(EX_IF_Latch.getIS_enable())
			{
				int newPC = EX_IF_Latch.getPC();
				containingProcessor.getRegisterFile().setProgramCounter(newPC);
				EX_IF_Latch.setIS_enable(false);
			}
			
            Simulator.getEventQueue().addEvent(
                new MemoryReadEvent(
					Clock.getCurrentTime() + containingProcessor.getL1iCache().getCacheLatency(),
					this,containingProcessor.getL1iCache(),
					containingProcessor.getRegisterFile().getProgramCounter()));
					
            IF_EnableLatch.setIF_busy(true);
			containingProcessor.getRegisterFile().setProgramCounter(containingProcessor.getRegisterFile().getProgramCounter() + 1);
			Statistics.setNumberOfInstructions(Statistics.getNumberOfInstructions() + 1);
        }
    }
    public void handleEvent(Event e) 
    {
        if(IF_OF_Latch.OF_busy)
        {
            e.setEventTime(Clock.getCurrentTime() + 1);
            Simulator.getEventQueue().addEvent(e);
        }
        else if (e.getEventType() == EventType.MemoryResponse)
        {
            MemoryResponseEvent event = (MemoryResponseEvent) e;
            int instruct = event.getValue();
            IF_OF_Latch.setInstruction(instruct);
            IF_OF_Latch.setOF_enable(true);
            IF_EnableLatch.setIF_busy(false);
            
            // setting operation type
			int operation_type = instruct >>> 27 ;
			OperationType[] operationType = OperationType.values();
			OperationType operation = operationType[operation_type];
        }
    }
}
