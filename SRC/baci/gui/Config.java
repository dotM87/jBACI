package baci.gui;

import java.io.*;
import java.awt.event.*;
import java.util.*;

/**
 * Configuration data
 * 
 * @author Moti Ben-Ari
 * @version 20 December 2002
 * See copyright.txt.
 */

public class Config {

// Software name and version

    public static final String SOFTWARE_NAME = "jBACI Concurrency Simulator V1.4.5";
    private static Properties properties = new Properties();

  // Some configuration data is final,
  // while some can be set from a property file config.cfg.
  // If config.cfg does not exist, a new one is written.

    private static void setDefaultProperties() {
	// Directories for source and compilers
	properties.put("SOURCE_DIRECTORY", "c:\\jbaci\\examples");
	properties.put("PASCAL_COMPILER", "c:\\jbaci\\bin\\bapas.exe");
	properties.put("C_COMPILER", "c:\\jbaci\\bin\\bacc.exe");

	// Should windows be shown initially?
	properties.put("INITIAL_CONSOLE", Boolean.toString(true));
	properties.put("INITIAL_HISTORY", Boolean.toString(false));
	properties.put("INITIAL_GLOBALS", Boolean.toString(true));
	properties.put("INITIAL_LINDA",   Boolean.toString(true));

	// Sizes of swing components
	properties.put("PROCESS_X", Integer.toString(300));   	// Process windows
	properties.put("PROCESS_Y", Integer.toString(350));
	properties.put("PTAB_X", Integer.toString(150));    	// Process table
	properties.put("PTAB_Y", Integer.toString(300));
	properties.put("SUBWINDOW_X", Integer.toString(300));   // Console, Global, History
	properties.put("SUBWINDOW_Y", Integer.toString(300));

	// Initial locations of windows
	properties.put("CONSOLE_X",   Integer.toString(0));
	properties.put("CONSOLE_Y",   Integer.toString(getIntegerProperty("PROCESS_Y")));
	properties.put("GLOBALS_X",   Integer.toString(getIntegerProperty("SUBWINDOW_X")));
	properties.put("GLOBALS_Y",   Integer.toString(getIntegerProperty("PROCESS_Y")));
	properties.put("HISTORY_X",   Integer.toString(2*getIntegerProperty("SUBWINDOW_X")));
	properties.put("HISTORY_Y",   Integer.toString(getIntegerProperty("PROCESS_Y")));
	properties.put("LINDA_X",     Integer.toString(2*getIntegerProperty("SUBWINDOW_X")));
	properties.put("LINDA_Y",     Integer.toString(getIntegerProperty("PROCESS_Y")));
	properties.put("INPUT_X",     Integer.toString(0));
	properties.put("INPUT_Y",     Integer.toString(0));
	
	// Default values of options
	properties.put("OPEN_MAIN_BY_DEFAULT", Boolean.toString(true));
	properties.put("STOP_ON_SWAP_DEFAULT", Boolean.toString(false));
	properties.put("SHOW_ACTIVE_DEFAULT",  Boolean.toString(true));
	properties.put("SHOW_HISTORY_SOURCE",  Boolean.toString(true));
	properties.put("WRITE_HISTORY_FILE",   Boolean.toString(false));

	// Sizes of swing components
	properties.put("FRAME_X", Integer.toString(800));  // Size of main frame
	properties.put("FRAME_Y", Integer.toString(600));  // Size of main frame
	properties.put("SOURCE_FONT_SIZE", Integer.toString(14));
	properties.put("PCODE_FONT_SIZE", Integer.toString(12)); 
	properties.put("TABLE_FONT_SIZE", Integer.toString(12)); 

	// Graphics canvas
	properties.put("G_XPOS",   Integer.toString(200));  // Location of canvas
	properties.put("G_YPOS",   Integer.toString(100));
	properties.put("G_WIDTH",  Integer.toString(600));  // Size of canvas
	properties.put("G_HEIGHT", Integer.toString(450));

	properties.put("TUPLE_SPACE", Integer.toString(5));
    }
  
// Fonts
    public static final String SOURCE_FONT_FAMILY = "Lucida Sans Typewriter";
    public static final int    SOURCE_FONT_STYLE  = java.awt.Font.PLAIN;

    public static final String PCODE_FONT_FAMILY  = "Lucida Sans";
    public static final int    PCODE_FONT_STYLE   = java.awt.Font.PLAIN;

    public static final int OFF_X   = 20;  // Offsets for successive subwindows (not used now)
    public static final int OFF_Y   = 30;
    
// Graphics canvas     
    public static final java.awt.Color G_COLOR  = java.awt.Color.white; // Color of canvas
    public static final String G_TITLE  = "jBACI Drawing Canvas";  // Title of canvas

// Swing component names, mnemonics and accelerators
    public static final String File                = "File";
    public static final int    FileMN              = KeyEvent.VK_F;
    public static final String New                 = "New";
    public static final int    NewMN               = KeyEvent.VK_N;
    public static final String NewAC              = "control N";
    public static final String Open                = "Open";
    public static final int    OpenMN              = KeyEvent.VK_O;
    public static final String OpenAC              = "control O";
    public static final String Save                = "Save";
    public static final int    SaveMN              = KeyEvent.VK_S;
    public static final String SaveAC              = "control S";
    public static final String SaveAs              = "Save as";
    public static final int    SaveAsMN           = KeyEvent.VK_A;
    public static final String SaveAsAC            = "control A";
    public static final String Exit                = "Exit";
    public static final int    ExitMN              = KeyEvent.VK_X;
    public static final String ExitAC              = "control Q";

    public static final String Editor              = "Editor";
    public static final int    EditorMN            = KeyEvent.VK_D;
    public static final String Copy                = "Copy";
    public static final int    CopyMN              = KeyEvent.VK_C;
    public static final String CopyAC              = "control C";
    public static final String Cut                 = "Cut";
    public static final int    CutMN               = KeyEvent.VK_U;
    public static final String CutAC               = "control X";
    public static final String Paste               = "Paste";
    public static final int    PasteMN             = KeyEvent.VK_P;
    public static final String PasteAC             = "control V";
    public static final String Find                = "Find";
    public static final int    FindMN              = KeyEvent.VK_I;
    public static final String FindAC              = "control F";
    public static final String FindAgain           = "Find again";
    public static final int    FindAgainMN         = KeyEvent.VK_A;
    public static final String FindAgainAC         = "control I";

    public static final String Program             = "Program";
    public static final int    ProgramMN           = KeyEvent.VK_M;
    public static final String Edit                = "Edit";
    public static final int    EditMN              = KeyEvent.VK_E;
    public static final String EditAC              = "control E";
    public static final String Compile             = "Compile";
    public static final int    CompileMN           = KeyEvent.VK_L;
    public static final String CompileAC           = "control L";
    public static final String Run                 = "Run";
    public static final int    RunMN               = KeyEvent.VK_R;
    public static final String RunAC               = "control R";
    public static final String Go                  = "Go";
    public static final int    GoMN                = KeyEvent.VK_G;
    public static final String GoAC                = "control G";
    public static final String Pause               = "Pause";
    public static final int    PauseMN             = KeyEvent.VK_P;
    public static final String PauseAC             = "ESCAPE";
    public static final String StepSource          = "Step Source";
    public static final int    StepSourceMN        = KeyEvent.VK_S;
    public static final String StepSourceAC        = "ENTER";
    public static final String StepPcode           = "Step Pcode";
    public static final int    StepPcodeMN         = KeyEvent.VK_T;
    public static final String StepPcodeAC         = "SPACE";

    public static final String Options             = "Options";
    public static final int    OptionsMN           = KeyEvent.VK_N;
    public static final String Active              = "Show Active Window";
    public static final int    ActiveMN            = KeyEvent.VK_S;
    public static final String Swap                = "Pause on Process Swap";
    public static final int    SwapMN              = KeyEvent.VK_P;
    public static final String HistorySource       = "History of Source Steps";
    public static final int    HistorySourceMN     = KeyEvent.VK_H;
    public static final String HistoryFile       = "Write History File";
    public static final int    HistoryFileMN     = KeyEvent.VK_W;

    public static final String Window              = "Window";
    public static final int    WindowMN            = KeyEvent.VK_W;
    public static final String Console             = "Console";
    public static final int    ConsoleMN           = KeyEvent.VK_C;
    public static final String History             = "History";
    public static final int    HistoryMN           = KeyEvent.VK_H;
    public static final String Globals             = "Globals";
    public static final int    GlobalsMN           = KeyEvent.VK_G;
    public static final String  LindaWindow        = "Linda";
    public static final int    LindaMN             = KeyEvent.VK_L;

    public static final String Help                = "Help";
    public static final int    HelpMN              = KeyEvent.VK_H;
    public static final String About               = "About";
    public static final int    AboutMN             = KeyEvent.VK_A;

    public static final String OpenProc            = "Display";
    public static final int    OpenProcMN          = KeyEvent.VK_Y;
    public static final String NextProc            = "Next";
    public static final int    NextProcMN          = KeyEvent.VK_X;
    public static final String PrevProc            = "Previous";
    public static final int    PrevProcMN          = KeyEvent.VK_U;

    public static final String AddSourceBreak      = "Add";
    public static final int    AddSourceBreakMN    = KeyEvent.VK_A;
    public static final String RemoveSourceBreak   = "Remove";
    public static final int    RemoveSourceBreakMN = KeyEvent.VK_D;
    public static final String AddPcodeBreak       = "Add";
    public static final int    AddPcodeBreakMN     = KeyEvent.VK_D;
    public static final String RemovePcodeBreak    = "Remove";
    public static final int    RemovePcodeBreakMN  = KeyEvent.VK_V;
    
    public static final String CodeTab             = "Code";
    public static final int    CodeTabMN           = KeyEvent.VK_C;
    public static final String ConsoleTab          = "Console";
    public static final int    ConsoleTabMN        = KeyEvent.VK_L;
    public static final String DetailsTab          = "Details";
    public static final int    DetailsTabMN        = KeyEvent.VK_I;

    public static final String PROC_PROCESS  = "Process";
    public static final String PROC_ACTIVE   = "Active";
    public static final String PROC_SUSPEND  = "Suspend";
    public static final String PROC_FINISH   = "Finish";
    public static final String PROC_MONITOR  = "Monitor";
    public static final String PROC_PRIORITY = "Pri";
    public static final String PROC_ATOMIC   = "Atm";

    public static final String PROC_TITLE    = "Processes";
    public static final String MRI_TITLE     = "Most recent instructions";
    public static final String HISTORY_TITLE = "History";
    public static final String CONSOLE_TITLE = "Console";
    public static final String GLOBALS_TITLE = "Globals";
    public static final String LINDA_TITLE   = "Linda Board";
    public static final String INPUT_TITLE   = "Input";

    public static final String PW_TITLE      = "Process state";
    public static final String PW_TOP        = "Top";
    public static final String PW_BOTTOM     = "Bottom";
    public static final String PW_ACTIVE     = "Active";
    public static final String PW_SUSPEND    = "Suspend";
    public static final String PW_FINISHED   = "Finished";
    public static final String PW_MONITOR    = "Monitor";
    public static final String PW_ATOMIC     = "Atomic";
    public static final String PW_PRIORITY   = "Priority";
    public static final String PW_PCODE      = "PCode";
    public static final String PW_SOURCE     = "Source";
    public static final String PW_STACK      = "Stack";
    public static final String PW_VARIABLES  = "Variables";
    public static final String PW_PROCESS    = "Process";

    public static final String STACK_INDEX   = "Index";
    public static final String STACK_VALUE   = "Value";
    public static final String VAR_NAME      = "Name";
    public static final String VAR_VALUE     = "Value";
    public static final String SOURCE_STATUS = "Status";
    public static final String SOURCE_CODE   = "Code";
    public static final String PCODE_STATUS  = "Status";
    public static final String PCODE_PCODE   = "PCode";
    public static final String MRI_PROC      = "Proc";
    public static final String MRI_FILE      = "File";
    public static final String MRI_LINE      = "Line";
    public static final String MRI_PCODE     = "PCode";
    public static final String MRI_SOURCE    = "Source";

    // Properties from configuration file

    public static void init() {
	  setDefaultProperties();
	  FileInputStream in = null;
	  try {
		  in = new FileInputStream("config.cfg");
	  }
	  catch (FileNotFoundException e1) {
		  System.out.println("Cannot open config.cfg; creating new file");
		  try {
			  FileOutputStream out = new FileOutputStream("config.cfg");
			  properties.store(out, "jBACI configuration file");
			  out.close();
			  in = new FileInputStream("config.cfg");
		  }
		  catch (IOException e2) {
			  System.out.println("Cannot write config.cfg");
		  } // catch
	  } // catch
	  try {
		  properties.load(in);
		  in.close();
	  }
	  catch (IOException e3) {
		  System.out.println("Cannot read config.cfg");
	  }
    } // init

  public static String getStringProperty(String s) {
	  return properties.getProperty(s);
  }

  public static boolean getBooleanProperty(String s) {
	  return Boolean.valueOf(properties.getProperty(s)).booleanValue();
  }
  
  public static int getIntegerProperty(String s) {
	  return Integer.valueOf(properties.getProperty(s)).intValue();
  }

}
