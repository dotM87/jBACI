package baci.program;

import baci.exception.*;
import java.io.*;
import java.util.*;

/**
 * Represents a source code file. Contains information from pcode file and source file.
 * @author: David Strite
 */
public class InputFile {
	//Index of parent file
	private int parent;
	//file name
	private String name;
	//Source code
	private ArrayList sourceCode;
/**
 * Constructs a new InputFile with specified parameters
 * @param name of file
 * @param parent index of parent input file
 * @param file File object to read data from
 */
 
public InputFile(String name, int parent, File file) throws BaciException {
	this.name = name;
	this.parent = parent;

	//Open input file
	BufferedReader inputFile;
	try {
		inputFile = new BufferedReader(new FileReader(file));
	} catch (FileNotFoundException fnfe) {
		throw new BaciException(fnfe.getMessage());
	}

	try {
		String line;
		sourceCode = new ArrayList();
		StringBuffer sb;
		int lastIndex;
		while(inputFile.ready()) {
			line = inputFile.readLine();
			if(line==null) {
				break;
			}
			//Convert tabs to spaces so they are displayed properly on the screen
			sb = new StringBuffer(line);
			lastIndex = line.length();
			lastIndex = line.lastIndexOf('\t',lastIndex-1);
			while(lastIndex!=-1) {
				sb.replace(lastIndex,lastIndex+1,"      ");
				lastIndex = line.lastIndexOf('\t',lastIndex-1);
			}
			sourceCode.add(sb.toString());
		}
	} catch (IOException ioe) {
		throw new BaciException(ioe.getMessage());
	}
	
}
/**
 * Gets the name of this file
 * @return String
 */
 
public String getName() {
	return name;
}
/**
 * Get the index of the parent of this InputFile
 * @return int
 */
public int getParent() {
	return parent;
}
/**
 * Get an ArrayList containing the source code for this file
 * @return ArrayList
 */
public ArrayList getSourceCode() {
	return sourceCode;
}
}
