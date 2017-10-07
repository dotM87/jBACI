package baci.interpreter;

/**
 * Represents a break point
 * @author: David Strite
 */
public class Breakpoint {
	//Constants for break point types
	public static final char TYPE_SOURCE = 'S';
	public static final char TYPE_TEMP = 'T';
	public static final char TYPE_PCODE = 'P';
	//type of breakpoint
	private char type;
	//pcode index where break point is
	private int pcodeIndex;
	//process this break point applies to (only for TYPE_TEMP break points)
	private int processIndex;
/**
 * Cronstructs a new Breakpoint with specified parameters
 * @param pcodeIndex
 * @param type
 */
public Breakpoint(int pcodeIndex, char type) {
	this(pcodeIndex,type,-1);
}
/**
 * Cronstructs a new Breakpoint with specified parameters
 * @param pcodeIndex
 * @param type
 * @param processIndex
 */
public Breakpoint(int pcodeIndex, char type, int processIndex) {
	this.pcodeIndex = pcodeIndex;
	this.type = type;
	this.processIndex = processIndex;
}
/**
 * Gets pcode address where this break point is
 * @return int
 */
public int getPcodeIndex() {
	return pcodeIndex;
}
/**
 * Gets process index for the process this break point applies to
 * @return int
 */
public int getProcessIndex() {
	return processIndex;
}
/**
 * Gets break point type
 * @return char
 */
public char getType() {
	return type;
}
}
