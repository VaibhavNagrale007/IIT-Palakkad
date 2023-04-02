package processor.memorysystem;

public class CacheLine {
    int data;
    int tag;
    // cache line has fields data and tag
    public CacheLine(){
        this.tag = -1;
        this.data = -1;
    }

    public int get_data() { return this.data; }
    // returning data
    public void set_data(int new_data){ this.data = new_data; }
	// setting data
    public int get_tag(){  return this.tag; }
	// returning tag
    public void set_tag(int new_tag){ this.tag = new_tag; }
    // setting tag

}
