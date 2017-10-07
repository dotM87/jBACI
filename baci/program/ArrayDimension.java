package baci.program;

/**
 * Represents an array dimension as read from pcode file
 * @author: David Strite
 */
 
public class ArrayDimension {
	//data type for indexes
	private int indexType;
	//data type for this dimension
	private int elementType;
	//if elements are arrays, will be index into array table
	private int elementRef;
	//Lowest index in array dimension
	private int lowIndex;
	//Highest index in array dimension
	private int highIndex;
	//Size of elements
	private int elementSize;
	//Total size of array
	private int size;
/**
 * Cronstructs a new ArrayDimension with specified values
 * @param indexType data type of indexes
 * @param elementType data type of elements
 * @param elementRef array index if elements are arrays
 * @param lowIndex lowest index
 * @param highIndex highest index
 * @param elementSize size of elements
 * @param size total size of array
 * @see Identifier for data type constants
 */
 
public ArrayDimension(int indexType, int elementType, int elementRef, int lowIndex, int highIndex, int elementSize, int size) {
	this.indexType = indexType;
	this.elementType = elementType;
	this.elementRef = elementRef;
	this.lowIndex = lowIndex;
	this.highIndex = highIndex;
	this.elementSize = elementSize;
	this.size = size;
}
/**
 * Gets array table index if elements of array are arrays
 * @return int
 */
 
public int getElementRef() {
	return elementRef;
}
/**
 * Gets size of elements
 * @return int
 */
 
public int getElementSize() {
	return elementSize;
}
/**
 * Gets data type of elements
 * @return int
 * @see Identifier for data type constants
 */
public int getElementType() {
	return elementType;
}
/**
 * Gets the highest index of the array
 * @return int
 */
public int getHighIndex() {
	return highIndex;
}
/**
 * Gets the data type of the array indexes
 * @return int
 * @see Identifier for data type constants
 */
public int getIndexType() {
	return indexType;
}
/**
 * Gets lowest index of array
 * @return int
 */
public int getLowIndex() {
	return lowIndex;
}
/**
 * Gets total size of array
 * @return int
 */
public int getSize() {
	return size;
}
}
