package baci.interpreter;

import baci.gui.treetable.*;
import baci.gui.Config;
import baci.program.*;
import java.util.*;
/**
 * Table model for variables data
 * @author David Strite
 */

public class VariableModel extends AbstractTreeTableModel {
	//Column Names
	static protected String[] cNames = {Config.VAR_NAME, Config.VAR_VALUE};
	//Column Classes
	static protected Class[] cTypes = {TreeTableModel.class, Object.class};
	
	private BaciProcess process;

	//Tree node Class
	public class VariableNode {
		//parent node
		private VariableNode parent;
		//children nodes
		protected ArrayList children;
		private Identifier identifier;
		private int stackIndex;
		private int arrayIndex;

		//Construct VariableNode
		public VariableNode(Identifier identifier, int stackIndex, int arrayIndex) { 
		    this(null, identifier, stackIndex, arrayIndex);
		}

		//Construct VariableNode
		public VariableNode(VariableNode parent, Identifier identifier, int stackIndex, int arrayIndex) { 
		    this.parent = parent;
		    this.identifier = identifier;
		    this.stackIndex = stackIndex;
		    this.arrayIndex = arrayIndex;
		    children = new ArrayList();
		}

		public int getArrayIndex() {
			return arrayIndex;
		}

		public int getStackIndex() {
			return stackIndex;
		}

		public Identifier getIdentifier() {
			return identifier;
		}

		public VariableNode getParent() {
		    return parent;
		}

		public ArrayList getChildren() {
		    return children; 
		}

		//Get path for this node
		public VariableNode[] getPath() {
		    return getPathToRoot(this, 0);
		}

		public VariableNode[] getPathToRoot(VariableNode aNode, int depth) {
		    VariableNode[] retNodes;

		    if(aNode == null) {
				if(depth == 0)
					return null;
				else
					retNodes = new VariableNode[depth];
		    } else {
				depth++;
				retNodes = getPathToRoot(aNode.getParent(), depth);
				retNodes[retNodes.length - depth] = aNode;
		    }
		    return retNodes;
		}

		//Set children of this node
		public void setChildren(ArrayList newChildren,boolean generateEvent) {
	    	children = newChildren;

	    	if (generateEvent) {
				VariableNode[] path = getPath();

				fireTreeStructureChanged(VariableModel.this, path, null,null);

				VariableNode parent = getParent();
			}
	    }
		
		public boolean isLeaf() {
			return children.size()==0;
		}

		public void nodeChanged() {
		    VariableNode parent = getParent();

		    if (parent != null) {
				VariableNode[] path = parent.getPath();
				int[] index = { getIndexOfChild(parent, this) };
				Object[] children = { this };

				fireTreeNodesChanged(VariableModel.this, path, index, children);
		    } else {
			    VariableNode[] path = getPath();

			    fireTreeStructureChanged(VariableModel.this, path, null, null);
		    }
		}

		public String toString() {
	    	if(identifier!=null) {
		    	return identifier.getName();
	    	}
			if(arrayIndex!=-1) {
				return String.valueOf(arrayIndex);
			}
			return "Variables...";
		}
	}
/**
 * Constructs a new VariableModel with specified process
 * @param process
 */
public VariableModel(BaciProcess process) { 
	super(null);
	root = new VariableNode(null,-1,-1);
	this.process = process;
}
/**
 * Gets the specified child of the specified node
 * @param node
 * @param i index
 * @return Object
 */
public Object getChild(Object node, int i) { 
	return getChildren(node).get(i); 
}
/**
 * Gets the number of children child of the specified node
 * @param node
 * @return int
 */
public int getChildCount(Object node) { 
	return getChildren(node).size();
}
/**
 * Gets the children of the specified node
 * @param node
 * @return ArrayList
 */
protected ArrayList getChildren(Object node) {
	return ((VariableNode)node).getChildren(); 
}
/**
 * Gets the Class for the specified column
 * @param column
 * @return Class
 */
public Class getColumnClass(int column) {
	return cTypes[column];
}
/**
 * Gets the number of columns in the table
 * @return int
 */
public int getColumnCount() {
	return cNames.length;
}
/**
 * Gets the name of the specified column
 * @param column
 * @return String
 */
public String getColumnName(int column) {
	return cNames[column];
}
/**
 * Finds a node for the specified stack index starting at root node
 * @param index
 * @return VariableNode
 */
public VariableNode getNodeForStackIndex(int index) {
	return getNodeForStackIndex((VariableNode)root,index);
}
/**
 * Finds a node for the specified stack index starting at specified node
 * @param noden
 * @param index
 * @return VariableNode
 */
protected VariableNode getNodeForStackIndex(VariableNode node, int index) {
	VariableNode child;
	for(int i=getChildCount(node)-1; i>=0; i--) {
		child = (VariableNode)getChild(node,i);
		if(child.getStackIndex()==index) {
			return child;
		} else {
			child = getNodeForStackIndex(child,index);
			if(child!=null) {
				return child;
			}
		}
			
	}
	return null;
}
/**
 * Gets the process associated with this VariableModel
 * @return BaciProcess
 */
public BaciProcess getProcess() {
	return process;
}
/**
 * Gets value for specified node and column
 * @param node
 * @param column
 * @return Object
 */
public Object getValueAt(Object node, int column) {
	VariableNode vn = (VariableNode)node;

    switch(column) {
	    case 0:
	    	vn.toString();
	    case 1:
	    	Identifier identifier = vn.getIdentifier();
	    	if(identifier!=null && identifier.getObj()==Identifier.OBJ_TYPE_MONITOR) {
		    	return "Monitor...";
	    	}
	    	if(identifier!=null && identifier.getTyp()==Identifier.DATA_TYPE_ARRAYS) {
		    	return "Array...";
	    	}
			int arrayIndex = vn.getArrayIndex();
	    	int stackIndex = vn.getStackIndex();
			if(arrayIndex!=-1 && stackIndex==-1) {
				return "Array...";
			}
	    	if(stackIndex!=-1) {
		    	if(identifier!=null && identifier.getTyp()==Identifier.DATA_TYPE_CHARS) {
			    	Object value = process.getStack().get(stackIndex);
			    	if(value!=null) {
				    	value = new Character((char)((Integer)value).intValue());
			    	}
				    return value;
		    	} else {
			    	return process.getStack().get(stackIndex);
		    	}
	    	}
	    	return null;
    }
	return null; 
}
/**
 * Determines if specified node is a leaf node
 * @param node
 * @return boolean
 */
public boolean isLeaf(Object node) {
	return ((VariableNode)node).isLeaf();
}
/**
 * Constructs a new VariableNode with specified parameters
 * @param parent
 * @param identifier
 * @param stackIndex
 * @param arrayIndex
 * @return VariableNode
 */
public VariableNode newNode(VariableNode parent, Identifier identifier, int stackIndex, int arrayIndex) {
	return new VariableNode(parent,identifier,stackIndex,arrayIndex);
}
}
