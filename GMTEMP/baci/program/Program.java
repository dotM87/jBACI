package baci.program;

import baci.exception.*;
import baci.interpreter.*;
import java.io.*;
import java.util.*;

/**
 * Stores information related to pcode and source files
 *
 * @author David Strite
 * @author Moti Ben-Ari
 * Modified December 2002 to add pcode instructions for graphics.
 * See copyright.txt.
 */

public class Program {
    //Data loaded from pcode file
    private ArrayList pcodeTable;
    private ArrayList identifierTable;
    private ArrayList blockTable;
    private ArrayList arrayTable;
    private HashMap stringTable;
    private ArrayList inputFileTable;
    private ArrayList mapping;
    private ArrayList codeBlockTable;
/**
 * Constructs a new Program using the specified pcode file
 *
 * @param pcodeFile the pcode file to parse
 * @throws BaciException when pcode file is invalid
 */
public Program(File pcodeFile) throws BaciException {
    BufferedReader inputFile;
    try {
        inputFile = new BufferedReader(new FileReader(pcodeFile));
    } catch (FileNotFoundException fnfe) {
        throw new BaciException(fnfe.getMessage());
    }

    try {
        String line;
        StringTokenizer lineTokens;
        int firstIndex, lastIndex;

        //Header
        line = inputFile.readLine();
        if(!line.startsWith("BACI ")) {
            throw new BaciException("Invalid header format: does not start with \"BACI\"");
        }
        //Discard second line of header
        line = inputFile.readLine();

        //Pcode Table
        line = inputFile.readLine();
        lineTokens = new StringTokenizer(line);
        firstIndex = Integer.valueOf(lineTokens.nextToken()).intValue();
        if(firstIndex!=0) {
            throw new BaciException("Pcode Table first index not 0");
        }
        lastIndex = Integer.valueOf(lineTokens.nextToken()).intValue();
        //Discard header line
        line = inputFile.readLine();
        int lc, f, x, y;
        PcodeInstruction instruction;
        pcodeTable = new ArrayList(lastIndex+1);
        for(int i=firstIndex; i<=lastIndex; i++) {
            line = inputFile.readLine();
            lineTokens = new StringTokenizer(line);
            lc = Integer.valueOf(lineTokens.nextToken()).intValue();
            if(i!=lc) {
                throw new BaciException("Pcode Table index error, "+i+"<>"+lc);
            }
            f = Integer.valueOf(lineTokens.nextToken()).intValue();
            x = Integer.valueOf(lineTokens.nextToken()).intValue();
            y = Integer.valueOf(lineTokens.nextToken()).intValue();
            instruction = createInstruction(f,x,y);
            if(instruction==null) {
                throw new BaciException(f+" is not a valid pcode instruction");
            }
            pcodeTable.add(instruction);
        }

        //Identifier table
        line = inputFile.readLine();
        lineTokens = new StringTokenizer(line);
        firstIndex = Integer.valueOf(lineTokens.nextToken()).intValue();
        if(firstIndex!=1) {
            throw new BaciException("Identifier Table first index not 1");
        }
        lastIndex = Integer.valueOf(lineTokens.nextToken()).intValue();
        //Discard header line
        line = inputFile.readLine();
        int index, link, obj, type, ref, normal, lev, adr, mon, atomic;
        String name;
        Identifier identifier;
        identifierTable = new ArrayList(lastIndex+1);
        identifierTable.add(null);
        for(int i=firstIndex; i<=lastIndex; i++) {
            line = inputFile.readLine();
            lineTokens = new StringTokenizer(line);
            index = Integer.valueOf(lineTokens.nextToken()).intValue();
            if(i!=index) {
                throw new BaciException("Identifier Table index error, "+i+"<>"+index);
            }
            name = lineTokens.nextToken();
            link = Integer.valueOf(lineTokens.nextToken()).intValue();
            obj = Integer.valueOf(lineTokens.nextToken()).intValue();
            type = Integer.valueOf(lineTokens.nextToken()).intValue();
            ref = Integer.valueOf(lineTokens.nextToken()).intValue();
            normal = Integer.valueOf(lineTokens.nextToken()).intValue();
            lev = Integer.valueOf(lineTokens.nextToken()).intValue();
            adr = Integer.valueOf(lineTokens.nextToken()).intValue();
            mon = Integer.valueOf(lineTokens.nextToken()).intValue();
            atomic = Integer.valueOf(lineTokens.nextToken()).intValue();
            identifier = new Identifier(name,link,obj,type,ref,normal!=0,lev,adr,mon,atomic!=0);
            identifierTable.add(identifier);
        }

        //Block Table
        line = inputFile.readLine();
        lineTokens = new StringTokenizer(line);
        firstIndex = Integer.valueOf(lineTokens.nextToken()).intValue();
        if(firstIndex!=0) {
            throw new BaciException("Block Table first index not 0");
        }
        lastIndex = Integer.valueOf(lineTokens.nextToken()).intValue();
        //Discard header line
        line = inputFile.readLine();
        int last, lastpar, psize, vsize;
        Block block;
        blockTable = new ArrayList(lastIndex+1);
        for(int i=firstIndex; i<=lastIndex; i++) {
            line = inputFile.readLine();
            lineTokens = new StringTokenizer(line);
            index = Integer.valueOf(lineTokens.nextToken()).intValue();
            if(i!=index) {
                throw new BaciException("Block Table index error "+i+"<>"+index);
            }
            last = Integer.valueOf(lineTokens.nextToken()).intValue();
            lastpar = Integer.valueOf(lineTokens.nextToken()).intValue();
            psize = Integer.valueOf(lineTokens.nextToken()).intValue();
            vsize = Integer.valueOf(lineTokens.nextToken()).intValue();
            block = new Block(last, lastpar, psize, vsize);
            blockTable.add(block);
        }

        //Array Table
        line = inputFile.readLine();
        lineTokens = new StringTokenizer(line);
        firstIndex = Integer.valueOf(lineTokens.nextToken()).intValue();
        if(firstIndex!=0) {
            throw new BaciException("String Table first index not 0");
        }
        lastIndex = Integer.valueOf(lineTokens.nextToken()).intValue();
        //Discard header line
        line = inputFile.readLine();
        int indexType, elementType, elementRef, lowIndex, highIndex, elementSize, size;
        ArrayDimension array;
        arrayTable = new ArrayList(lastIndex+1);
        for(int i=firstIndex; i<=lastIndex; i++) {
            line = inputFile.readLine();
            lineTokens = new StringTokenizer(line);
            index = Integer.valueOf(lineTokens.nextToken()).intValue();
            if(i!=index) {
                throw new BaciException("Array Table index error, "+i+"<>"+index);
            }
            indexType = Integer.valueOf(lineTokens.nextToken()).intValue();
            elementType = Integer.valueOf(lineTokens.nextToken()).intValue();
            elementRef = Integer.valueOf(lineTokens.nextToken()).intValue();
            lowIndex = Integer.valueOf(lineTokens.nextToken()).intValue();
            highIndex = Integer.valueOf(lineTokens.nextToken()).intValue();
            elementSize = Integer.valueOf(lineTokens.nextToken()).intValue();
            size = Integer.valueOf(lineTokens.nextToken()).intValue();
            array = new ArrayDimension(indexType, elementType, elementRef, lowIndex, highIndex, elementSize, size);
            arrayTable.add(array);
        }

        //String Table
        int gulpSize;
        line = inputFile.readLine();
        lineTokens = new StringTokenizer(line);
        firstIndex = Integer.valueOf(lineTokens.nextToken()).intValue();
        if(firstIndex!=0) {
            throw new BaciException("String Table first index not 0");
        }
        lastIndex = Integer.valueOf(lineTokens.nextToken()).intValue();
        gulpSize = Integer.valueOf(lineTokens.nextToken()).intValue();

        StringBuffer allStrings = new StringBuffer(lastIndex);
        while(allStrings.length()<lastIndex) {
            line = inputFile.readLine();
            allStrings.append(line);
            if(allStrings.length()%gulpSize!=0 && allStrings.length()!=lastIndex) {
                allStrings.append('\n');
            }
        }
        StringTokenizer allStringsTokens = new StringTokenizer(allStrings.toString(),"\0");
        int begin = 0;
        String string;
        stringTable = new HashMap();
        while(allStringsTokens.hasMoreElements()) {
            string = allStringsTokens.nextToken();
            stringTable.put(new Integer(begin), string);
            begin+=string.length()+1;
        }

        //Input File Table
        line = inputFile.readLine();
        lineTokens = new StringTokenizer(line);
        firstIndex = Integer.valueOf(lineTokens.nextToken()).intValue();
        if(firstIndex!=0) {
            throw new BaciException("Input File Table first index not 0");
        }
        lastIndex = Integer.valueOf(lineTokens.nextToken()).intValue();
        //Discard header line
        line = inputFile.readLine();
        int parent;
        InputFile file;
        inputFileTable = new ArrayList(lastIndex+1);
        for(int i=firstIndex; i<=lastIndex; i++) {
            line = inputFile.readLine();
            lineTokens = new StringTokenizer(line);
            index = Integer.valueOf(lineTokens.nextToken()).intValue();
            if(i!=index) {
                throw new BaciException("Input File Table index error, "+i+"<>"+index);
            }
            parent = Integer.valueOf(lineTokens.nextToken()).intValue();
            name = lineTokens.nextToken();
	// Top level contains full path; add parent for include files
	    if (parent == -1)
                file = new InputFile(name,parent,new File(name));
            else
                file = new InputFile(name,parent,new File(pcodeFile.getParent(),name));
            inputFileTable.add(file);
        }

        int numFiles = inputFileTable.size();
        int numLines;
        ArrayList fileArrayList;
        mapping = new ArrayList(numFiles);
        for(int i=0; i<numFiles; i++) {
            numLines = ((InputFile)inputFileTable.get(i)).getSourceCode().size();
            fileArrayList = new ArrayList(numLines);
            mapping.add(fileArrayList);
            for(int j=0; j<numLines; j++) {
                fileArrayList.add(new ArrayList());
            }
        }

        //Debug data
        line = inputFile.readLine();
        lineTokens = new StringTokenizer(line);
        firstIndex = Integer.valueOf(lineTokens.nextToken()).intValue();
        if(firstIndex!=0) {
            throw new BaciException("Debug Data first index not 0");
        }
        lastIndex = Integer.valueOf(lineTokens.nextToken()).intValue();
        //Discard header line
        line = inputFile.readLine();

        int[] pcodeIndexes = new int[lastIndex+1];
        int[] fileIndexes = new int[lastIndex+1];
        int[] fileLineIndexes = new int[lastIndex+1];

        for(int i=firstIndex; i<=lastIndex; i++) {
            line = inputFile.readLine();
            lineTokens = new StringTokenizer(line);
            pcodeIndexes[i] = Integer.valueOf(lineTokens.nextToken()).intValue();
            fileIndexes[i] = Integer.valueOf(lineTokens.nextToken()).intValue();
            fileLineIndexes[i] = Math.abs(Integer.valueOf(lineTokens.nextToken()).intValue());
        }

        ArrayList lineArrayList;
        for(int i=pcodeIndexes.length-2; i>=0; i--) {
            lineArrayList = (ArrayList)((ArrayList)mapping.get(fileIndexes[i])).get(fileLineIndexes[i]-1);
            for(int j=pcodeIndexes[i]; j<pcodeIndexes[i+1]; j++) {
                lineArrayList.add(new Integer(j));
                ((PcodeInstruction)pcodeTable.get(j)).setFileIndex(fileIndexes[i]);
                ((PcodeInstruction)pcodeTable.get(j)).setFileLineIndex(fileLineIndexes[i]-1);
            }
        }

        TreeSet tempCodeBlockTable = new TreeSet();
        //Determine block for pcode
        for(int i=identifierTable.size()-1; i>=1; i--) {
            identifier = getIdentifier(i);
            obj = identifier.getObj();
            if(obj==Identifier.OBJ_TYPE_PROCEDURE || obj==Identifier.OBJ_TYPE_FUNCTION ||
                obj==Identifier.OBJ_TYPE_MONITOR || obj==Identifier.OBJ_TYPE_OUTER_BLOCK ||
                obj==Identifier.OBJ_TYPE_MAINPROC) {

                tempCodeBlockTable.add(new CodeBlock(i,identifier.getRef(),identifier.getAdr()));
            }
        }
        codeBlockTable = new ArrayList(tempCodeBlockTable);
        int pcodeIndex = 0;
        CodeBlock currentCodeBlock, nextCodeBlock;
        for(int i=0; i<codeBlockTable.size()-1; i++) {
            currentCodeBlock = getCodeBlock(i);
            nextCodeBlock = getCodeBlock(i+1);
            while(pcodeIndex<nextCodeBlock.getPcodeIndex()) {
                getPcode(pcodeIndex).setBlockIndex(currentCodeBlock.getBlockIndex());
                pcodeIndex++;
            }
        }
        CodeBlock lastCodeBlock = getCodeBlock(codeBlockTable.size()-1);
        for(int i=pcodeIndex; i<pcodeTable.size(); i++) {
            getPcode(i).setBlockIndex(lastCodeBlock.getBlockIndex());
        }
    } catch (IOException ioe) {
        throw new BaciException(ioe.getMessage());
    }

}
/**
 * Adds an array variable to the variable model and recurses if necessary
 * @param model the variable model to add the array variable to
 * @param parent parent node that the new nodes should be added to
 * @param array array dimension to process
 * @param bottom bottom of stack
 */
private void buildArray(VariableModel model, VariableModel.VariableNode parent, ArrayDimension array, int bottom) {
    int low = array.getLowIndex();
    int high = array.getHighIndex();
    int elementSize = array.getElementSize();
    for(int i=low; i<=high; i++) {
        int newBottom = bottom+(i-low)*elementSize;
        //If elements are also arrays, expand them as well
        if(array.getElementType()==Identifier.DATA_TYPE_ARRAYS) {
            VariableModel.VariableNode node = model.newNode(parent,null,-1,i);
            parent.getChildren().add(node);
            buildArray(model,node,getArray(array.getElementRef()),newBottom);
        } else {
            parent.getChildren().add(model.newNode(parent,null,newBottom,i));
        }
    }
}
/**
 * Adds variables in specified block to variable model
 * @model variable model to add variables to
 * @block block to process variables in
 * @bottom stack bottom
 */
public void buildVariables(VariableModel model, int block, int bottom) {
    VariableModel.VariableNode rootNode = (VariableModel.VariableNode)model.getRoot();
    rootNode.getChildren().clear();
    buildVariables(model,rootNode,block,bottom);
    rootNode.nodeChanged();
}
/**
 * Adds variables in specified block to variable model
 * @model variable model to add variables to
 * @node node to add new nodes too
 * @block block to process variables in
 * @bottom stack bottom
 */
private void buildVariables(VariableModel model, VariableModel.VariableNode node, int block, int bottom) {
    int index = getBlock(block).getLast();
    if(index==0) {
        //If there are no variables in this block, the last field may be 0
        return;
    }
    Identifier identifier = getIdentifier(index);
    if(identifier.getRef()==block) {
        //If there are no variables in this block, the first variable found may be the identifier
        //for the procedure/function
        switch (identifier.getObj()) {
            case Identifier.OBJ_TYPE_FUNCTION:
            case Identifier.OBJ_TYPE_MAINPROC:
            case Identifier.OBJ_TYPE_PROCEDURE:
                return;
        }
    }
    while(index>0) {
        if(identifier.getObj()==Identifier.OBJ_TYPE_VARIABLE) {
            switch (identifier.getTyp()) {
                case Identifier.DATA_TYPE_ARRAYS: {
                    //add node for this variable and call buildArray to add nodes for elements of array
                    VariableModel.VariableNode arrayNode = model.newNode(node,identifier,-1,-1);
                    node.getChildren().add(0,arrayNode);
                    ArrayDimension array = getArray(identifier.getRef());
                    buildArray(model,arrayNode,array,bottom+identifier.getAdr());
                    break;
                }
                case Identifier.DATA_TYPE_BOOLS:
                case Identifier.DATA_TYPE_BSEMS:
                case Identifier.DATA_TYPE_CHARS:
                case Identifier.DATA_TYPE_CONDS:
                case Identifier.DATA_TYPE_INTS:
                case Identifier.DATA_TYPE_NO_TYPE:
                case Identifier.DATA_TYPE_SEMS:
                case Identifier.DATA_TYPE_STRINGS: {
                    //add node for simple variable
                    node.getChildren().add(0,model.newNode(node,identifier,bottom+identifier.getAdr(),-1));
                    break;
                }
            }
        } else if(identifier.getObj()==Identifier.OBJ_TYPE_MONITOR) {
            if(node.getIdentifier()==null || node.getIdentifier().getObj()!=Identifier.OBJ_TYPE_MONITOR) {
                //a monitor was found, add a node for the monitor then call buildVariables to add member
                //variables to the model
                VariableModel.VariableNode monitorNode = model.newNode(node,identifier,-1,-1);
                node.getChildren().add(0,monitorNode);
//              buildVariables(model,monitorNode,identifier.getRef(),getBlock(blockTable.size()-1).getVsize()+getBlock(0).getVsize());
		int mainVsize = getBlock(getIdentifier(getBlock(0).getLastpar()).getRef()).getVsize();
                buildVariables(model,monitorNode,identifier.getRef(),mainVsize+getBlock(0).getVsize());
//		buildVariables(model,monitorNode,identifier.getRef(),identifier.getMon());  // David suggests
            }
        }
        index = identifier.getLink();
        identifier = getIdentifier(index);
    }
}
/**
 * Creates an instruction corresponding to the f value with x and y as parameters
 * @return baci.program.PcodeInstruction
 * @param f pcode function code
 * @param x pcode instruction parameter
 * @param y pcode instruction parameter
 */
private PcodeInstruction createInstruction(int f, int x, int y) {
    switch (f) {
        case 0:
            return new InstructionLoadAddress(this,x,y);
        case 1:
            return new InstructionLoadValue(this,x,y);
        case 2:
            return new InstructionLoadIndirect(this,x,y);
        case 3:
            return new InstructionUpdateDisplay(this,x,y);
        case 4:
            return new InstructionCobegin(this,x,y);
        case 5:
            return new InstructionCoend(this,x,y);
        case 6:
            return new InstructionWait(this,x,y);
        case 7:
            return new InstructionSignal(this,x,y);
        case 8:
            return new InstructionEnterMonitor(this,x,y);
        case 9:
            return new InstructionExitMonitor(this,x,y);
        case 10:
            return new InstructionWaitCondition(this,x,y);
        case 11:
            return new InstructionSignalCondition(this,x,y);
        case 12:
            return new InstructionEmpty(this,x,y);
        case 13:
            return new InstructionEolEof(this,x,y);
        case 14:
            return new InstructionJump(this,x,y);
        case 15:
            return new InstructionJumpZero(this,x,y);
        case 16:
            return new InstructionBeginFor(this,x,y);
        case 17:
            return new InstructionEndFor(this,x,y);
        case 18:
            return new InstructionMarkStack(this,x,y);
        case 19:
            return new InstructionCall(this,x,y);
        case 21:
            return new InstructionIndex(this,x,y);
        case 22:
            return new InstructionLoadBlock(this,x,y);
        case 23:
            return new InstructionCopyBlock(this,x,y);
        case 24:
            return new InstructionPushLiteral(this,x,y);
        case 27:
            return new InstructionRead(this,x,y);
        case 28:
            return new InstructionWriteRawString(this,x,y);
        case 29:
            return new InstructionWrite(this,x,y);
        case 31:
            return new InstructionHalt(this,x,y);
        case 32:
            return new InstructionExitProc(this,x,y);
        case 33:
            return new InstructionExitFunction(this,x,y);
        case 34:
            return new InstructionValueAt(this,x,y);
        case 35:
            return new InstructionComplement(this,x,y);
        case 36:
            return new InstructionNegate(this,x,y);
        case 37:
            return new InstructionStoreKeep(this,x,y);
        case 38:
            return new InstructionStore(this,x,y);
        case 39:
            return new InstructionStoreSemaphore(this,x,y);
        case 40:
            return new InstructionCallMonitorInit(this,x,y);
        case 41:
            return new InstructionReturnMonitorInit(this,x,y);
        case 44:
            return new InstructionTestEqualKeep(this,x,y);
        case 45:
            return new InstructionTestEQ(this,x,y);
        case 46:
            return new InstructionTestNE(this,x,y);
        case 47:
            return new InstructionTestLT(this,x,y);
        case 48:
            return new InstructionTestLE(this,x,y);
        case 49:
            return new InstructionTestGT(this,x,y);
        case 50:
            return new InstructionTestGE(this,x,y);
        case 51:
            return new InstructionDoOr(this,x,y);
        case 52:
            return new InstructionDoAdd(this,x,y);
        case 53:
            return new InstructionDoSub(this,x,y);
        case 56:
            return new InstructionDoAnd(this,x,y);
        case 57:
            return new InstructionDoMult(this,x,y);
        case 58:
            return new InstructionDoDiv(this,x,y);
        case 59:
            return new InstructionDoMod(this,x,y);
        case 62:
            return new InstructionReadln(this,x,y);
        case 63:
            return new InstructionWriteln(this,x,y);
        case 71:
            return new InstructionWhichProc(this,x,y);
        case 72:
            return new InstructionSuspend(this,x,y);
        case 73:
            return new InstructionRevive(this,x,y);
        case 74:
            return new InstructionRandom(this,x,y);
        case 80:
            return new InstructionShortCall(this,x,y);
        case 81:
            return new InstructionShortReturn(this,x,y);
        case 90:
            return new InstructionSscanf(this,x,y);
        case 91:
            return new InstructionSprintf(this,x,y);
        case 110:
            return new InstructionWriteString(this,x,y);
        case 111:
            return new InstructionCopyRawString(this,x,y);
        case 112:
            return new InstructionCopyString(this,x,y);
        case 113:
            return new InstructionConcatRawString(this,x,y);
        case 114:
            return new InstructionConcatString(this,x,y);
        case 115:
            return new InstructionCompareRawString(this,x,y);
        case 116:
            return new InstructionCompareString(this,x,y);
        case 117:
            return new InstructionLengthString(this,x,y);
        // Instructions for graphics
        case 120:
            return new InstructionCreate(this,x,y);
        case 121:
            return new InstructionMakeVisible(this,x,y);
        case 122:
            return new InstructionMoveTo(this,x,y);
        case 123:
            return new InstructionMoveBy(this,x,y);
        case 124:
            return new InstructionChangeColor(this,x,y);
         case 130:
            return new InstructionPost(this,x,y);
         case 131:
            return new InstructionRemove(this,x,y);
    }
    return null;
}
/**
 * Gets an array dimension from the array table
 * @return ArrayDimension
 * @param index for the array
 */
public ArrayDimension getArray(int index) {
    return (ArrayDimension)arrayTable.get(index);
}
/**
 * Gets a block from the block table
 * @return Block
 * @param index for the block
 */
public Block getBlock(int index) {
    return (Block)blockTable.get(index);
}
/**
 * Gets a code block from the code block
 * @return CodeBlock
 * @param index for the code block
 */
public CodeBlock getCodeBlock(int index) {
    return (CodeBlock)codeBlockTable.get(index);
}
/**
 * Gets an input file from the input file table
 * @return InputFile
 * @param index for file
 */
public InputFile getFile(int index) {
    return (InputFile)inputFileTable.get(index);
}
/**
 * Gets an ArrayList containing the source code for an input file
 * @return ArrayList
 * @param index of the file
 */
public ArrayList getFileSource(int i) {
    return ((InputFile)inputFileTable.get(i)).getSourceCode();
}
/**
 * Gets an Identifier from the identifier table
 * @return Identifier
 * @param index of the identifier
 */
public Identifier getIdentifier(int index) {
    return (Identifier)identifierTable.get(index);
}
/**
 * Finds an Identifier in specified block at specified level and offset
 * @return Identifier
 * @param block
 * @param level
 * @param offset
 */
public Identifier getIdentifier(int block, int level, int offset) {
    int codeBlockIndex = -1;
    CodeBlock codeBlock;

    if(offset==0 && level>0) {
        for(codeBlockIndex=0; codeBlockIndex<codeBlockTable.size(); codeBlockIndex++) {
            codeBlock = getCodeBlock(codeBlockIndex);
            if(codeBlock.getBlockIndex()==block) {
                return getIdentifier(codeBlock.getIdentifierIndex());
            }
        }
    } else {
        int tempBlock = block;
        while(codeBlockIndex < codeBlockTable.size()) {
            int index = getBlock(tempBlock).getLast();
            Identifier ident=null;
            while(index > 0) {
                ident = getIdentifier(index);
                if(ident.getLevel()==level && ident.getAdr()==offset && (ident.getObj()==Identifier.OBJ_TYPE_VARIABLE || ident.getObj()==Identifier.OBJ_TYPE_EXT_VARIABLE)) {
                    return ident;
                }
                index = ident.getLink();
            }
            do {
                codeBlockIndex++;
                codeBlock = getCodeBlock(codeBlockIndex);
                ident = getIdentifier(codeBlock.getIdentifierIndex());
            } while(codeBlock.getBlockIndex()==block || !(ident.getLevel()<level || ident.getObj()==Identifier.OBJ_TYPE_OUTER_BLOCK));
            tempBlock = codeBlock.getBlockIndex();
        }
    }
    return null;
}
/**
 * Gets an ArrayList of the pcode instruction indexes associated with specified file and line
 * @return ArrayList
 * @param fileIndex index of file
 * @param lineIndex line number
 */
private ArrayList getMapping(int fileIndex, int lineIndex) {
    return (ArrayList)((ArrayList)mapping.get(fileIndex)).get(lineIndex);
}
/**
 * Gets the first pcode instruction indexe associated with specified file and line
 * @return int
 * @param fileIndex index of file
 * @param lineIndex line number
 */
public int getMappingFirst(int fileIndex, int lineIndex) {
    ArrayList lineMapping = getMapping(fileIndex,lineIndex);
    if(lineMapping.size()>0) {
        return ((Integer)lineMapping.get(0)).intValue();
    } else {
        return -1;
    }
}
/**
 * Gets the last pcode instruction indexe associated with specified file and line
 * @return int
 * @param fileIndex index of file
 * @param lineIndex line number
 */
public int getMappingLast(int fileIndex, int lineIndex) {
    ArrayList lineMapping = getMapping(fileIndex,lineIndex);
    if(lineMapping.size()>0) {
        return ((Integer)lineMapping.get(lineMapping.size()-1)).intValue();
    } else {
        return -1;
    }
}
/**
 * Gets the PcodeInstruction at the specified index
 * @return PcodeInstruction
 * @param i index
 */
public PcodeInstruction getPcode(int i) {
    return (PcodeInstruction)pcodeTable.get(i);
}
/**
 * Gets the number of pcode instructions
 * @return int
 */
public int getPcodeSize() {
    return pcodeTable.size();
}
/**
 * Gets the String from the string table at the specified index
 * @return String
 * @param index for string
 */
public String getString(int index) {
    return (String)stringTable.get(new Integer(index));
}
/**
 * Determines if a given pcode instruction index is valid
 * @param index for pcode instruction
 * @throws BaciException if index is invalid
 */
public void validatePcodeAddress(int index) throws BaciException {
    if(index < 0 || index >= pcodeTable.size()) {
        throw new BaciException("Invalid pcode address: "+index);
    }
}
}
