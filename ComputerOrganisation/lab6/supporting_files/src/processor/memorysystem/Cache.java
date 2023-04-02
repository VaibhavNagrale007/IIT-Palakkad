package processor.memorysystem;
import configuration.Configuration;
import generic.*;
import processor.Clock;
import processor.Processor;
import processor.pipeline.MemoryAccess;


public class Cache implements Element {
    Processor containingProcessor;
    int Latency;
    CacheLine[] Cache_arr;
    int lines_num;
    int addr_cache_miss;
    Element elem_cache_miss;
    Boolean read_op;
    int write_data;

    public Cache(Processor processor, int size, int latency){
        this.containingProcessor = processor;
        this.lines_num = size / 4;
		this.Latency = latency;
		// initializing cache_arr 
        Cache_arr = new CacheLine[lines_num];
        for(int i = 0; i < lines_num; i++)
            Cache_arr[i] = new CacheLine();
    }

    public int extract_inst(int n,int i,int j){	// for getting address extracting instruction
        n = n << i;
        n = n >>> j;
        return n;
    }
    
    public int getCacheLatency() { return Latency; }
    
    public void cacheRead(int addr, Element requestingElement){
        int cache_addr;
        int indexBits = (int) (Math.log(lines_num) / Math.log(2)); //calculating the index bits which is log(no_of _lines) base 2
        if(indexBits == 0) {                                       // index bits is 0 assign cache address to zero
            cache_addr = 0;
        }
        else{ 
                                                             //if index bits are non zero
             cache_addr = extract_inst( addr_cache_miss, 32-indexBits, 32-indexBits); //getting the cache address from the address
        }
        int tagOfCache = Cache_arr[cache_addr].get_tag(); //tag 
        if(tagOfCache == addr){    
                               //checking the address with tag if equal -->> hit
             Simulator.getEventQueue().addEvent(
                     new MemoryResponseEvent(
                             Clock.getCurrentTime(),
                             this,requestingElement,Cache_arr[cache_addr].get_data()
                    )
            );
        }
        
        else{            
                                       //if tag is not equal to address -->> cache miss
             read_op = true;
             handleCacheMiss(addr, requestingElement);
        }
        
    }
    
	public void cacheWrite(int addr, int value, Element requestingElement){
        int cache_addr;
        int indexBits = (int) (Math.log(lines_num) / Math.log(2));  //calculating the index bits which is log(no_of _lines) base 2
        
        if(indexBits == 0){ 
                                                // index bits is 0 assign cache address to zero
            cache_addr = 0;
        }
        
        else{              
            cache_addr = extract_inst( addr_cache_miss, 32-indexBits, 32-indexBits);//getting the cache address from the address
        }
        int tagOfCache= Cache_arr[cache_addr].get_tag();//tag 
        if(tagOfCache == addr){      
                                       //checking the address with tag if equal -->> hit
            Cache_arr[cache_addr].set_data(value);
            Simulator.getEventQueue().addEvent(
                    new MemoryWriteEvent(
                            Clock.getCurrentTime(),
                            this,containingProcessor.getMainMemory(),addr,value
                    )
            );
             ((MemoryAccess)requestingElement).EX_MA_Latch.setMA_busy(false);
             ((MemoryAccess)requestingElement).MA_RW_Latch.setRW_enable(true);
        }
        else{      
                                                           //if tag is not equal to address -->> cache miss              
            read_op = false;
            write_data = value;
            handleCacheMiss(addr, requestingElement);
        }
	
    }
    
	public void handleCacheMiss(int addr, Element requestingElement){
        // requests the main memory for the given line
        Simulator.getEventQueue().addEvent(
                new MemoryReadEvent(
                        Clock.getCurrentTime() + Configuration.mainMemoryLatency,
                        this,containingProcessor.getMainMemory(),addr
                )
        );
        addr_cache_miss = addr;
        elem_cache_miss = requestingElement;
    }

	public void handleResponse(int value){
        // handles the response from the lower level in the memory
        int indexBits = (int) (Math.log(lines_num) / Math.log(2));
        int cache_addr;
        if(indexBits == 0){
            cache_addr = 0;
        }
        else{
            cache_addr = extract_inst( addr_cache_miss, 32-indexBits, 32-indexBits);
        }
        // System.out.println("index: "+indexBits+"cacheMiss: "+addr_cache_miss+"cache_addr: "+cache_addr);
        // System.out.println(" New : "+((addr_cache_miss<<(32-indexBits))>>>(32 - indexBits)));
        // System.out.println("elem: "+elem_cache_miss);
        
        Cache_arr[cache_addr].set_data(value);
        Cache_arr[cache_addr].set_tag(addr_cache_miss);
        if(read_op){
            Simulator.getEventQueue().addEvent(
                    new MemoryResponseEvent(
                            Clock.getCurrentTime(),
                            this,elem_cache_miss,value
                    )
            );
        }
        else{
            cacheWrite(addr_cache_miss, write_data, elem_cache_miss);
        }
    }
    
    @Override
    public void handleEvent(Event e) {	// for handling event
        // if event is read type
        if(e.getEventType() == Event.EventType.MemoryRead){
            MemoryReadEvent event = (MemoryReadEvent) e;
            cacheRead(event.getAddressToReadFrom(), event.getRequestingElement());
        }
        // if it is memory response event
        else if(e.getEventType() == Event.EventType.MemoryResponse){
            MemoryResponseEvent event = (MemoryResponseEvent) e;
            handleResponse(event.getValue());
        }
        // if event is write type
        else if(e.getEventType() == Event.EventType.MemoryWrite){
            MemoryWriteEvent event = (MemoryWriteEvent) e;
            cacheWrite(event.getAddressToWriteTo(), event.getValue(), event.getRequestingElement());
        }
    }
}
