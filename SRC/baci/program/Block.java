package baci.program;

/**
 * Represents a Block as read from pcode file
 * @author: David Strite
 */
public class Block {
	//Index of last identifier in this block
	private int last;
	//Index of last parameter in this block
	private int lastpar;
	//stack frame size of this block
	private int psize;
	//total size of this block
	private int vsize;
/**
 * Constructs a new Block with specified parameters
 * @param last index of last identifier in this block
 * @param lastpar index of last parameter in this block
 * @param psize stack frame size of this block
 * @param vsize the total size of this block
 */
public Block(int last, int lastpar, int psize, int vsize) {
	this.last = last;
	this.lastpar = lastpar;
	this.psize = psize;
	this.vsize = vsize;
}
/**
 * Gets the index of the last identifier in this block
 * @return int
 */
public int getLast() {
	return last;
}
/**
 * Gets the index of the last parameter in this block
 * @return int
 */
public int getLastpar() {
	return lastpar;
}
/**
 * Gets the stack frame size of this block
 * @return int
 */
public int getPsize() {
	return psize;
}
/**
 * Gets the total size of this block
 * Creation date: (6/20/2001 9:17:02 PM)
 * @return int
 */
public int getVsize() {
	return vsize;
}
}
