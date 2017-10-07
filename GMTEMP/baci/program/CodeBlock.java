package baci.program;

/**
 * Represents a block of code
 * @author: David Strite
 */
public class CodeBlock implements Comparable {
	//Index of the identifier of this code block
	private int identifierIndex;
	//Index of the block table of this code block
	private int blockIndex;
	//Index of the first instruction in this code block
	private int pcodeIndex;
/**
 * Constructs a new CodeBlock with the specified parameters
 * @param iIndex index of identifier for this code block
 * @param bIndex index of block for this code block
 * @param pIndex index of first pcode instruction in this code block
 */
public CodeBlock(int iIndex, int bIndex, int pIndex) {
	identifierIndex = iIndex;
	blockIndex = bIndex;
	pcodeIndex = pIndex;
}
/**
 * Compares another code block with this code block
 * Returns 0 if pcodeIndexes are equal
 * Returns -1 if the pcodeIndex of this block is less than pcodeIndex of the other block
 * Returns 1 if the pcodeIndex of this block is larger than pcodeIndex of the other block
 * @return int
 * @param o CodeBlock to compare to
 */
public int compareTo(Object o) {
	int otherIndex = ((CodeBlock)o).pcodeIndex;
	return (pcodeIndex<otherIndex ? -1 : (pcodeIndex==otherIndex ? 0 : 1));
}
/**
 * Compares two CodeBlocks returns true if the pcodeIndex are equal, false otherwise
 * @return boolean
 * @param o Object to compare to
 */
public boolean equals(Object o) {
	if(o==null || !(o instanceof CodeBlock)) {
		return false;
	}
	return this.pcodeIndex==((CodeBlock)o).pcodeIndex;
}
/**
 * Gets the index of the block for this code block
 * @return int
 */
public int getBlockIndex() {
	return blockIndex;
}
/**
 * Gets the index of the Identifier for this code block
 * @return int
 */
public int getIdentifierIndex() {
	return identifierIndex;
}
/**
 * Gets the index of the first PcodeInstruction in this code block
 * @return int
 */
public int getPcodeIndex() {
	return pcodeIndex;
}
}
