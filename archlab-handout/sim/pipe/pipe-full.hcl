#/* $begin pipe-all-hcl */
####################################################################
#    HCL Description of Control for Pipelined Y86-64 Processor     #
#    Copyright (C) Randal E. Bryant, David R. O'Hallaron, 2014     #
####################################################################

## Your task is to implement the iaddq instruction
## The file contains a declaration of the icodes
## for iaddq (IIADDQ)
## Your job is to add the rest of the logic to make it work

#
## Ali MAMMADOV, ali.mammadov

#--------------------------------------------
#                                           |
##  PC <-- valP                  PC UPDATE  |
#                                           |
#-------------------------------------------|
#                                           |
##  R[rB] <-- valE               WRITE BACK |
#                                           |
#-------------------------------------------|
#                                           |
##                               MEMORY     |
#                                           |
#-------------------------------------------|
#                                           |
##  valE <-- valB + valC         EXECUTE    |
##  Set CC                                  |
#                                           |
#-------------------------------------------|
#                                           |
##  valB <-- R[rB]               DECODE     |
#                                           |
#-------------------------------------------|
#                                           |
##  icode:ifun <-- M1[PC]                   |
##  rA:rB <-- M1[PC+1]                      |
##  valC  <-- M8[PC+2]           FETCH      |
##  valP  <-- PC + 10                       |
#                                           |
#--------------------------------------------

####################################################################
#    C Include's.  Don't alter these                               #
####################################################################

quote '#include <stdio.h>'
quote '#include "isa.h"'
quote '#include "pipeline.h"'
quote '#include "stages.h"'
quote '#include "sim.h"'
quote 'int sim_main(int argc, char *argv[]);'
quote 'int main(int argc, char *argv[]){return sim_main(argc,argv);}'

####################################################################
#    Declarations.  Do not change/remove/delete any of these       #
####################################################################

##### Symbolic representation of Y86-64 Instruction Codes #############
wordsig INOP 	'I_NOP'
wordsig IHALT	'I_HALT'
wordsig IRRMOVQ	'I_RRMOVQ'
wordsig IIRMOVQ	'I_IRMOVQ'
wordsig IRMMOVQ	'I_RMMOVQ'
wordsig IMRMOVQ	'I_MRMOVQ'
wordsig IOPQ	'I_ALU'
wordsig IJXX	'I_JMP'
wordsig ICALL	'I_CALL'
wordsig IRET	'I_RET'
wordsig IPUSHQ	'I_PUSHQ'
wordsig IPOPQ	'I_POPQ'
# Instruction code for iaddq instruction
wordsig IIADDQ	'I_IADDQ'

##### Symbolic represenations of Y86-64 function codes            #####
wordsig FNONE    'F_NONE'        # Default function code

##### Symbolic representation of Y86-64 Registers referenced      #####
wordsig RRSP     'REG_RSP'    	     # Stack Pointer
wordsig RNONE    'REG_NONE'   	     # Special value indicating "no register"

##### ALU Functions referenced explicitly ##########################
wordsig ALUADD	'A_ADD'		     # ALU should add its arguments

##### Possible instruction status values                       #####
wordsig SBUB	'STAT_BUB'	# Bubble in stage
wordsig SAOK	'STAT_AOK'	# Normal execution
wordsig SADR	'STAT_ADR'	# Invalid memory address
wordsig SINS	'STAT_INS'	# Invalid instruction
wordsig SHLT	'STAT_HLT'	# Halt instruction encountered

##### Signals that can be referenced by control logic ##############

##### Pipeline Register F ##########################################

wordsig F_predPC 'pc_curr->pc'	     # Predicted value of PC

##### Intermediate Values in Fetch Stage ###########################

wordsig imem_icode  'imem_icode'      # icode field from instruction memory
wordsig imem_ifun   'imem_ifun'       # ifun  field from instruction memory
wordsig f_icode	'if_id_next->icode'  # (Possibly modified) instruction code
wordsig f_ifun	'if_id_next->ifun'   # Fetched instruction function
wordsig f_valC	'if_id_next->valc'   # Constant data of fetched instruction
wordsig f_valP	'if_id_next->valp'   # Address of following instruction
boolsig imem_error 'imem_error'	     # Error signal from instruction memory
boolsig instr_valid 'instr_valid'    # Is fetched instruction valid?

##### Pipeline Register D ##########################################
wordsig D_icode 'if_id_curr->icode'   # Instruction code
wordsig D_rA 'if_id_curr->ra'	     # rA field from instruction
wordsig D_rB 'if_id_curr->rb'	     # rB field from instruction
wordsig D_valP 'if_id_curr->valp'     # Incremented PC

##### Intermediate Values in Decode Stage  #########################

wordsig d_srcA	 'id_ex_next->srca'  # srcA from decoded instruction
wordsig d_srcB	 'id_ex_next->srcb'  # srcB from decoded instruction
wordsig d_rvalA 'd_regvala'	     # valA read from register file
wordsig d_rvalB 'd_regvalb'	     # valB read from register file

##### Pipeline Register E ##########################################
wordsig E_icode 'id_ex_curr->icode'   # Instruction code
wordsig E_ifun  'id_ex_curr->ifun'    # Instruction function
wordsig E_valC  'id_ex_curr->valc'    # Constant data
wordsig E_srcA  'id_ex_curr->srca'    # Source A register ID
wordsig E_valA  'id_ex_curr->vala'    # Source A value
wordsig E_srcB  'id_ex_curr->srcb'    # Source B register ID
wordsig E_valB  'id_ex_curr->valb'    # Source B value
wordsig E_dstE 'id_ex_curr->deste'    # Destination E register ID
wordsig E_dstM 'id_ex_curr->destm'    # Destination M register ID

##### Intermediate Values in Execute Stage #########################
wordsig e_valE 'ex_mem_next->vale'	# valE generated by ALU
boolsig e_Cnd 'ex_mem_next->takebranch' # Does condition hold?
wordsig e_dstE 'ex_mem_next->deste'      # dstE (possibly modified to be RNONE)

##### Pipeline Register M                  #########################
wordsig M_stat 'ex_mem_curr->status'     # Instruction status
wordsig M_icode 'ex_mem_curr->icode'	# Instruction code
wordsig M_ifun  'ex_mem_curr->ifun'	# Instruction function
wordsig M_valA  'ex_mem_curr->vala'      # Source A value
wordsig M_dstE 'ex_mem_curr->deste'	# Destination E register ID
wordsig M_valE  'ex_mem_curr->vale'      # ALU E value
wordsig M_dstM 'ex_mem_curr->destm'	# Destination M register ID
boolsig M_Cnd 'ex_mem_curr->takebranch'	# Condition flag
boolsig dmem_error 'dmem_error'	        # Error signal from instruction memory

##### Intermediate Values in Memory Stage ##########################
wordsig m_valM 'mem_wb_next->valm'	# valM generated by memory
wordsig m_stat 'mem_wb_next->status'	# stat (possibly modified to be SADR)

##### Pipeline Register W ##########################################
wordsig W_stat 'mem_wb_curr->status'     # Instruction status
wordsig W_icode 'mem_wb_curr->icode'	# Instruction code
wordsig W_dstE 'mem_wb_curr->deste'	# Destination E register ID
wordsig W_valE  'mem_wb_curr->vale'      # ALU E value
wordsig W_dstM 'mem_wb_curr->destm'	# Destination M register ID
wordsig W_valM  'mem_wb_curr->valm'	# Memory M value

####################################################################
#    Control Signal Definitions.                                   #
####################################################################

################ Fetch Stage     ###################################

## What address should instruction be fetched at
word f_pc = [
	# Mispredicted branch.  Fetch at incremented PC
	M_icode == IJXX && !M_Cnd : M_valA;
	# Completion of RET instruction
	W_icode == IRET : W_valM;
	# Default: Use predicted value of PC
	1 : F_predPC;
];

## Determine icode of fetched instruction
word f_icode = [
	imem_error : INOP;
	1: imem_icode;
];

# Determine ifun
word f_ifun = [
	imem_error : FNONE;
	1: imem_ifun;
];

# Is instruction valid?
bool instr_valid = f_icode in 
	{ INOP, IHALT, IRRMOVQ, IIRMOVQ, IRMMOVQ, IMRMOVQ,
	  IOPQ, IJXX, ICALL, IRET, IPUSHQ, IPOPQ, IIADDQ };

# Determine status code for fetched instruction
word f_stat = [
	imem_error: SADR;
	!instr_valid : SINS;
	f_icode == IHALT : SHLT;
	1 : SAOK;
];

# Does fetched instruction require a regid byte?
bool need_regids =
	f_icode in { IRRMOVQ, IOPQ, IPUSHQ, IPOPQ, 
		     IIRMOVQ, IRMMOVQ, IMRMOVQ, IIADDQ };

# Does fetched instruction require a constant word?
bool need_valC =
	f_icode in { IIRMOVQ, IRMMOVQ, IMRMOVQ, IJXX, ICALL, IIADDQ };

# Predict next value of PC
word f_predPC = [
	f_icode in { IJXX, ICALL } : f_valC;
	1 : f_valP;
];

################ Decode Stage ######################################


## What register should be used as the A source?
word d_srcA = [
	D_icode in { IRRMOVQ, IRMMOVQ, IOPQ, IPUSHQ  } : D_rA;
	D_icode in { IPOPQ, IRET } : RRSP;
	1 : RNONE; # Don't need register
];

## What register should be used as the B source?
word d_srcB = [
	D_icode in { IOPQ, IRMMOVQ, IMRMOVQ, IIADDQ  } : D_rB;
	D_icode in { IPUSHQ, IPOPQ, ICALL, IRET } : RRSP;
	1 : RNONE;  # Don't need register
];

## What register should be used as the E destination?
word d_dstE = [
	D_icode in { IRRMOVQ, IIRMOVQ, IOPQ, IIADDQ} : D_rB;
	D_icode in { IPUSHQ, IPOPQ, ICALL, IRET } : RRSP;
	1 : RNONE;  # Don't write any register
];

## What register should be used as the M destination?
word d_dstM = [
	D_icode in { IMRMOVQ, IPOPQ } : D_rA;
	1 : RNONE;  # Don't write any register
];

## What should be the A value?
## Forward into decode stage for valA
word d_valA = [
	D_icode in { ICALL, IJXX } : D_valP; # Use incremented PC
	d_srcA == e_dstE : e_valE;    # Forward valE from execute
	d_srcA == M_dstM : m_valM;    # Forward valM from memory
	d_srcA == M_dstE : M_valE;    # Forward valE from memory
	d_srcA == W_dstM : W_valM;    # Forward valM from write back
	d_srcA == W_dstE : W_valE;    # Forward valE from write back
	1 : d_rvalA;  # Use value read from register file
];

word d_valB = [
	d_srcB == e_dstE : e_valE;    # Forward valE from execute
	d_srcB == M_dstM : m_valM;    # Forward valM from memory
	d_srcB == M_dstE : M_valE;    # Forward valE from memory
	d_srcB == W_dstM : W_valM;    # Forward valM from write back
	d_srcB == W_dstE : W_valE;    # Forward valE from write back
	1 : d_rvalB;  # Use value read from register file
];

################ Execute Stage #####################################

## Select input A to ALU
word aluA = [
	E_icode in { IRRMOVQ, IOPQ } : E_valA;
	E_icode in { IIRMOVQ, IRMMOVQ, IMRMOVQ, IIADDQ } : E_valC;
	E_icode in { ICALL, IPUSHQ } : -8;
	E_icode in { IRET, IPOPQ } : 8;
	# Other instructions don't need ALU
];

## Select input B to ALU
word aluB = [
	E_icode in { IRMMOVQ, IMRMOVQ, IOPQ, ICALL, 
		     IPUSHQ, IRET, IPOPQ, IIADDQ} : E_valB;
	E_icode in { IRRMOVQ, IIRMOVQ } : 0;
	# Other instructions don't need ALU
];

## Set the ALU function
word alufun = [
	E_icode == IOPQ : E_ifun;
	1 : ALUADD;
];

## Should the condition codes be updated?
bool set_cc = E_icode in { IOPQ, IIADDQ } &&
	# State changes only during normal operation
	!m_stat in { SADR, SINS, SHLT } && !W_stat in { SADR, SINS, SHLT };

## Generate valA in execute stage
word e_valA = E_valA;    # Pass valA through stage

## Set dstE to RNONE in event of not-taken conditional move
word e_dstE = [
	E_icode == IRRMOVQ && !e_Cnd : RNONE;
	1 : E_dstE;
];

################ Memory Stage ######################################

## Select memory address
word mem_addr = [
	M_icode in { IRMMOVQ, IPUSHQ, ICALL, IMRMOVQ } : M_valE;
	M_icode in { IPOPQ, IRET } : M_valA;
	# Other instructions don't need address
];

## Set read control signal
bool mem_read = M_icode in { IMRMOVQ, IPOPQ, IRET };

## Set write control signal
bool mem_write = M_icode in { IRMMOVQ, IPUSHQ, ICALL };

#/* $begin pipe-m_stat-hcl */
## Update the status
word m_stat = [
	dmem_error : SADR;
	1 : M_stat;
];
#/* $end pipe-m_stat-hcl */

## Set E port register ID
word w_dstE = W_dstE;

## Set E port value
word w_valE = W_valE;

## Set M port register ID
word w_dstM = W_dstM;

## Set M port value
word w_valM = W_valM;

## Update processor status
word Stat = [
	W_stat == SBUB : SAOK;
	1 : W_stat;
];

################ Pipeline Register Control #########################

# Should I stall or inject a bubble into Pipeline Register F?
# At most one of these can be true.
bool F_bubble = 0;
bool F_stall =
	# Conditions for a load/use hazard
	E_icode in { IMRMOVQ, IPOPQ } &&
	 E_dstM in { d_srcA, d_srcB } ||
	# Stalling at fetch while ret passes through pipeline
	IRET in { D_icode, E_icode, M_icode };

# Should I stall or inject a bubble into Pipeline Register D?
# At most one of these can be true.
bool D_stall = 
	# Conditions for a load/use hazard
	E_icode in { IMRMOVQ, IPOPQ } &&
	 E_dstM in { d_srcA, d_srcB };

bool D_bubble =
	# Mispredicted branch
	(E_icode == IJXX && !e_Cnd) ||
	# Stalling at fetch while ret passes through pipeline
	# but not condition for a load/use hazard
	!(E_icode in { IMRMOVQ, IPOPQ } && E_dstM in { d_srcA, d_srcB }) &&
	  IRET in { D_icode, E_icode, M_icode };

# Should I stall or inject a bubble into Pipeline Register E?
# At most one of these can be true.
bool E_stall = 0;
bool E_bubble =
	# Mispredicted branch
	(E_icode == IJXX && !e_Cnd) ||
	# Conditions for a load/use hazard
	E_icode in { IMRMOVQ, IPOPQ } &&
	 E_dstM in { d_srcA, d_srcB};

# Should I stall or inject a bubble into Pipeline Register M?
# At most one of these can be true.
bool M_stall = 0;
# Start injecting bubbles as soon as exception passes through memory stage
bool M_bubble = m_stat in { SADR, SINS, SHLT } || W_stat in { SADR, SINS, SHLT };

# Should I stall or inject a bubble into Pipeline Register W?
bool W_stall = W_stat in { SADR, SINS, SHLT };
bool W_bubble = 0;
#/* $end pipe-all-hcl */
