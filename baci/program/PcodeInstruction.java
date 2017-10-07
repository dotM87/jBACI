package baci.program;

import baci.exception.*;
import baci.interpreter.*;

/**
 * Super class of all pcode instructions. Represents one pcode instruction.
 * @author: David Strite
 */
 
public abstract class PcodeInstruction {
	//Pcode instruction parameters
	private int x;
	private int y;
	//Source file and line that generated this pcode instruction
	private int fileIndex;
	private int fileLineIndex;
	//Program this instruction is a part of
	private Program program;
	//Block this instruction is in
	private int blockIndex;
	//Description of this instruction
	private String description;
/**
 * Construct a new PcodeInstruction belonging to specified Program with specified parameters
 * @param program this instruction is to be part of
 * @param x pcode instruction parameter
 * @param y pcode instruction parameter
 */
public PcodeInstruction(Program program, int x, int y) {
	this.program = program;
	this.x = x;
	this.y = y;
}
/**
 * Performs this pcode instruction's action
 * @param interpreter calling this instruction
 * @throws BaciException if there is an error
 */
public abstract void doInstruction(Interpreter interpreter) throws BaciException;
/**
 * Gets the block index this instruction is part of
 * @return int
 */
public int getBlockIndex() {
	return blockIndex;
}
/**
 * Gets the description of this instruction
 * @return String
 */
protected abstract String getDescription();
/**
 * Gets the index of the source file this instruction was created by
 * @return int
 */
public int getFileIndex() {
	return fileIndex;
}
/**
 * Gets the line number of the source file that created this instruction
 * @return int
 */
public int getFileLineIndex() {
	return fileLineIndex;
}
/**
 * Gets the Program this instruction is part of
 * @return Program
 */
public Program getProgram() {
	return program;
}
/**
 * Gets the instruction parameter
 * @return int
 */
public int getX() {
	return x;
}
/**
 * Gets the instruction parameter
 * @return int
 */
public int getY() {
	return y;
}
/**
 * Sets the block index this instruction is part of
 * @param newBlockIndex index of block
 */
public void setBlockIndex(int newBlockIndex) {
	blockIndex = newBlockIndex;
}
/**
 * Sets the index of the source file this instruction was created by
 * @param newFileIndex index of file
 */
public void setFileIndex(int newFileIndex) {
	fileIndex = newFileIndex;
}
/**
 * Sets the line number of the source file that created this instruction
 * @param newFileLineIndex file line number
 */
public void setFileLineIndex(int newFileLineIndex) {
	fileLineIndex = newFileLineIndex;
}
/**
 * Gets a string representation of this instruction
 * @return String
 */
public String toString() {
	if(description==null) {
		description=getDescription();
	}
	return description;
}
}
