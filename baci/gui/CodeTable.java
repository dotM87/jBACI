package baci.gui;

/*******************************************************************************
 * Copyright 2001,2002 David Strite
 *
 * This file is part of BACI Debugger.
 *
 * BACI Debugger is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 *
 * BACI Debugger is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 ******************************************************************************/

import baci.interpreter.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.table.*;

/**
 * Table for source and pcode
 * @author: David Strite
 */
public class CodeTable extends BaciTable {
	/**
	 * Construct new CodeTable
	 */
	public CodeTable() {
		super();
	}
	
	/**
	 * Called when table data is changed
	 * @param e event
	 */
	public void tableChanged(TableModelEvent e) {
		super.tableChanged(e);
		TableModel model = getModel();
		if(model instanceof CodeModel) {
			//Make sure current line is visible
			CodeModel cModel = (CodeModel)model;
			makeCellVisible(cModel.getCurrentRow(),0);
		}
		if(model instanceof BaciTableModel) {
			//resize column to width of widest data
			int maxWidth = ((BaciTableModel)getModel()).getMaxWidth(1,getFontMetrics(getFont()));
			getColumnModel().getColumn(1).setPreferredWidth(maxWidth);
		}
	}
}
