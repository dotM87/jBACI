package baci.program;

import baci.interpreter.*;

/**
 * Represents an Identifier as read from the pcode file
 * @author: Administrator
 */
 
public class Identifier {
	//Name of this identifier
	private String name;
	//Index of next identifier at this level
	private int link;
	//Type of identifier
	private int obj;
	//Data type of identifier
	private int typ;
	//Index into block table or array table
	private int ref;
	//false for pass by reference parms, true for others
	private boolean normal;
	//Level of identifier
	private int level;
	//Entry point for a proc/function
	private int adr;
	//Index of monitor for variables in monitor
	private int mon;
	//true for atomic proc/function, false otherwise
	private boolean atomic;

	//Types of identifiers
	public static final int OBJ_TYPE_CONSTANT = 0;
	public static final int OBJ_TYPE_VARIABLE = 1;
	public static final int OBJ_TYPE_TYPE = 2;
	public static final int OBJ_TYPE_PROCEDURE = 3;
	public static final int OBJ_TYPE_FUNCTION = 4;
	public static final int OBJ_TYPE_MONITOR = 5;
	public static final int OBJ_TYPE_MAINPROC = 6;
	public static final int OBJ_TYPE_OUTER_BLOCK = 7;
	public static final int OBJ_TYPE_EXT_PROCEDURE = 8;
	public static final int OBJ_TYPE_EXT_FUNCTION = 9;
	public static final int OBJ_TYPE_EXT_MONITOR = 10;
	public static final int OBJ_TYPE_EXT_VARIABLE = 11;

	//Data types of identifiers
	public static final int DATA_TYPE_NO_TYPE = 0;
	public static final int DATA_TYPE_INTS = 1;
	public static final int DATA_TYPE_BOOLS = 2;
	public static final int DATA_TYPE_CHARS = 3;
	public static final int DATA_TYPE_BSEMS = 4;
	public static final int DATA_TYPE_SEMS = 5;
	public static final int DATA_TYPE_CONDS = 6;
	public static final int DATA_TYPE_ARRAYS = 7;
	public static final int DATA_TYPE_RAW_STRINGS = 10;
	public static final int DATA_TYPE_STRINGS = 11;
/**
 * Constructs a new Identifier with specified parms
 * @param name of identifier
 * @param link index of next identifier at this level
 * @param obj type of identifier
 * @param typ data type of identifier
 * @param ref index of block or array depending on obj type
 * @param normal is parameter pass by value
 * @param level of identifier
 * @param adr entry point for proc/function
 * @param mon index of monitor for variables in monitor
 * @param atomic is proc/function atomic
 */
public Identifier(String name, int link, int obj, int typ, int ref, boolean normal, int level, int adr, int mon, boolean atomic) {
	this.name = name;
	this.link = link;
	this.obj = obj;
	this.typ = typ;
	this.ref = ref;
	this.normal = normal;
	this.level = level;
	this.adr = adr;
	this.mon = mon;
	this.atomic = atomic;
}
/**
 * Gets entry point for a proc/function
 * @return int
 */
public int getAdr() {
	return adr;
}
/**
 * Gets level of this identifier
 * @return int
 */
public int getLevel() {
	return level;
}
/**
 * Gets index of next variable at this level
 * @return int
 */
public int getLink() {
	return link;
}
/**
 * Gets index of monitor if variable is in a monitor
 * @return int
 */
public int getMon() {
	return mon;
}
/**
 * Gets name of this identifier
 * @return String
 */
public String getName() {
	return name;
}
/**
 * Get type of this identifier
 * @return int
 */
public int getObj() {
	return obj;
}
/**
 * Gets block index or array index depending on object type
 * @return int
 */
public int getRef() {
	return ref;
}
/**
 * Gets data type of this Identifier
 * @return int
 */
public int getTyp() {
	return typ;
}
/**
 * Determines if this Identifier is an atomic proc/function
 * @return boolean
 */
public boolean isAtomic() {
	return atomic;
}
/**
 * Determines if this Identifier is a normal or a pass by reference parameter
 * @return boolean
 */
public boolean isNormal() {
	return normal;
}
/**
 * Sets monitor index for this Identifier
 * @param newMon identifier index
 */
public void setMon(int newMon) {
	mon = newMon;
}
}
