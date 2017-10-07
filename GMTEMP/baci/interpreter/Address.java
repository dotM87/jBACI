package baci.interpreter;

/**
 * Stores an stack address
 * @author: David Strite
 */
public class Address {
	//process owning stack
	private BaciProcess process;
	//stack index
	private int index;
/**
 * Constructions a new Address with specified parameters
 * @param BaciProcess owning the stack
 * @param index into stack
 */
public Address(BaciProcess process, int index) {
	this.process = process;
	this.index = index;
}
/**
 * Determines if this stack is equal to another
 * @return boolean
 * @param o to compare to
 */
public boolean equals(Object o) {
	if(o instanceof Address) {
		Address a = (Address)o;
		return index==a.index && process.equals(a.process);
		
	}
	return false;
}
/**
 * Gets the stack index of this Address
 * @return int
 */
public int getIndex() {
	return index;
}
/**
 * Gets the process owning the stack
 * @return BaciProcess
 */
public BaciProcess getProcess() {
	return process;
}
/**
 * Sets the stack index
 * @param newIndex 
 */
public void setIndex(int newIndex) {
	index = newIndex;
}
/**
 * Sets the process owning the stack
 * @param newProcess
 */
public void setProcess(BaciProcess newProcess) {
	process = newProcess;
}
/**
 * Gets a string representation of this Address
 * @return String
 */
public String toString() {
	return "Address["+process.getIndex()+","+index+"]";
}
}
