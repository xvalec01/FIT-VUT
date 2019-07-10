import getopt
import re
import sys
import xml.etree.ElementTree as ETree


def exitMessageError(text, errorNum):
    print(text, file=sys.stderr)
    sys.exit(errorNum)


def parseXml(file):
    string = ""
    for line in file:
        string += line

    try:
        xml = ETree.fromstring(string)
    except:
        exitMessageError("ERROR: XML syntax is invalid", 31)

    try:
        language = xml.attrib["language"]
        program = xml.tag
    except:
        exitMessageError("ERROR: XML header is invalid", 32)

    if len(xml.attrib) != 1 or language.upper() != "IPPCODE19" or program != "program":
        exitMessageError("ERROR: XML header is invalid", 32)

    xmlDictOrder = {}
    orderControl = []

    for instruction in xml:
        if instruction.attrib["order"] not in orderControl:
            orderControl.append(instruction.attrib["order"])

        else:
            exitMessageError("ERROR: Two orders with same number.", 32)
        xmlDictInstr = {}
        xmlDictArgs = {}
        for arguments in instruction:
            if not arguments.attrib["type"]:
                exitMessageError("ERROR: Order not found ", 32)

            xmlListValues = []
            xmlListValues.append(arguments.attrib["type"])
            xmlListValues.append(arguments.text)

            arg = arguments.tag[0:3]
            argNum = arguments.tag[-1]


            if (arg == "arg") and (argNum.isdigit()):
                xmlDictArgs[int(argNum)] = xmlListValues

            else:
                exitMessageError("ERROR: Name of tag is invalid", 32)


        if not instruction.attrib["opcode"]:
            exitMessageError("ERROR: Opcode not found ", 32)
        xmlDictInstr[instruction.attrib["opcode"].upper()] = xmlDictArgs
        if not instruction.attrib["order"]:
            exitMessageError("ERROR: Order not found ", 32)
        xmlDictOrder[int(instruction.attrib["order"])] = xmlDictInstr

    return xmlDictOrder


def getNested(data, *args):
    if args and data:
        element = args[0]
        if element:
            value = data.get(element)
            return value if len(args) == 1 else getNested(value, *args[1:])


def isVarDefined(typeAndValue, TF, LF, GF):
    if typeAndValue[0] == "TF":
        if TF is None:
            exitMessageError("ERROR: No temporary frame defined for [" + typeAndValue[0] + "@" + typeAndValue[1] + "]",
                             54)
        if typeAndValue[1] not in TF:
            exitMessageError("ERROR: Variable [" + typeAndValue[0] + "@" + typeAndValue[1] + "] not defined.", 54)

    elif typeAndValue[0] == "LF":

        if LF is None:
            exitMessageError("ERROR: No local frame defined for [" + typeAndValue[0] + "@" + typeAndValue[1] + "]", 55)
        if typeAndValue[1] not in LF:
            exitMessageError("ERROR: Variable [" + typeAndValue[0] + "@" + typeAndValue[1] + "] is not in local frame.",
                             54)

    elif typeAndValue[0] == "GF":

        if typeAndValue[1] not in GF:
            exitMessageError("ERROR: Variable [" + typeAndValue[0] + "@" + typeAndValue[1] + "] is not defined.", 54)


def isArgsCountOk(argsDict, instruction):
    noOperand = ["CREATEFRAME", "PUSHFRAME", "POPFRAME", "RETURN", "BREAK"]
    oneOperand = ["DEFVAR", "CALL", "PUSHS", "POPS", "WRITE", "LABEL", "JUMP", "EXIT", "DPRINT"]
    twoOperands = ["MOVE", "INT2CHAR", "READ", "STRLEN", "TYPE", "NOT"]
    threeOperands = ["ADD", "SUB", "MUL", "IDIV", "LT", "GT", "EQ", "AND", "OR", "STRI2INT", "CONCAT", "GETCHAR",
                     "SETCHAR", "JUMPIFEQ", "JUMPIFNEQ"]

    if instruction in noOperand:
        if len(argsDict) == 0:
            return True
        else:
            exitMessageError("ERROR: Wrong number of arguments, it should be [0]", 32)
    if instruction in oneOperand:
        if len(argsDict) == 1:
            arguments = set(sorted(argsDict))
            if 1 not in arguments:
                exitMessageError("ERROR: Wrong number in arguments",32)
            return True
        else:
            exitMessageError("ERROR: Wrong number of arguments, it should be [1]", 32)
    elif instruction in twoOperands:
        if len(argsDict) == 2:
            arguments = set(sorted(argsDict))
            if 1 not in arguments or 2 not in arguments:
                exitMessageError("ERROR: Wrong number in arguments", 32)
            return True
        else:
            exitMessageError("ERROR: Wrong number of arguments, it should be [2]", 32)
    elif instruction in threeOperands:
        if len(argsDict) == 3:
            arguments = set(sorted(argsDict))
            if 1 not in arguments or 2 not in arguments or 3 not in arguments:
                exitMessageError("ERROR: Wrong number in arguments", 32)
            return True
        else:
            exitMessageError("ERROR: Wrong number of arguments, it should be [3]", 32)
    else:
        exitMessageError("ERROR: Wrong number of arguments", 32)


def varTypeCheck(typeAndValue):
    if typeAndValue[0] != "var":
        exitMessageError("ERROR: Type is not ", 32)

    frames = ["GF", "TF", "LF"]
    frameType = typeAndValue[1].split("@", 1)

    if not (frameType[0] in frames and re.search("^([_\-$&%*!?]|[a-zA-Z])([_\-$&%*!?]|[a-zA-Z0-9])*$", frameType[1])):
        exitMessageError("ERROR: Variable syntax error. " + typeAndValue[1], 32)
    return frameType


def labelTypeCheck(typeAndValue):
    if typeAndValue[0] != "label":
        exitMessageError("Error: Type is not 'label' but, " + typeAndValue[0], 32)
    if not re.search("^([_\-$&%*!?]|[a-zA-Z])([_\-$&%*!?]|[a-zA-Z0-9])*$", typeAndValue[1]):
        exitMessageError("ERROR: Label syntax error. " + typeAndValue[1], 32)


def symbCheck(typeAndValue, TF, LF, GF):

    if typeAndValue[0] == "nil":
        if typeAndValue[0] == "nil" and typeAndValue[1] != "nil":
            exitMessageError("ERROR: Expect special value nil@nil", 56)
        return typeAndValue
    elif typeAndValue[0] == "var":
        variable = varTypeCheck(typeAndValue)
        isVarDefined(variable, TF, LF, GF)
        return variable

    elif typeAndValue[0] == "string":
        if typeAndValue[1] is None:
            typeAndValue[1] = ""
            return typeAndValue

        if re.search(r"(\\\\\D{1,3})|(\\\\\d{1,2}\D)|(\\\\\d{1,2}$)|(\\\\$)|(#)", typeAndValue[1]):
            exitMessageError("ERROR: Wrong value of string found", 32)

        typeAndValue[1] = re.sub(r"\\\\x5c([0-9][0-9][0-9])", lambda w: chr(int(w.group(1))), typeAndValue[1])


    elif typeAndValue[0] == "int":
        return typeAndValue
    elif typeAndValue[0] == "bool":
        return typeAndValue
    else:
        exitMessageError("ERROR: Only types nil, bool, string or int.", 32)
    return typeAndValue


def valueToVariable(frameAndVariableName, typeAndValue, TF, LF, GF):
    if frameAndVariableName[0] != "string" and typeAndValue[0] is None:
        exitMessageError("ERROR: Wrong value [" + typeAndValue[0] + "]", 56)

    if typeAndValue[0] == "string" and typeAndValue[0] is None:
        typeAndValue[1] = ""

    if frameAndVariableName[0] == "TF":
        TF[frameAndVariableName[1]] = typeAndValue
    elif frameAndVariableName[0] == "LF":
        LF[frameAndVariableName[1]] = typeAndValue
    elif frameAndVariableName[0] == "GF":
        GF[frameAndVariableName[1]] = typeAndValue


def getValueFromVariable(typeAndValue, TF, LF, GF, ):

    if typeAndValue[0] == "TF":

        requiredValue = TF[typeAndValue[1]]
    elif typeAndValue[0] == "LF":

        requiredValue = LF[typeAndValue[1]]
    elif typeAndValue[0] == "GF":

        requiredValue = GF[typeAndValue[1]]
    else:
        requiredValue = typeAndValue
    if  requiredValue[0] != "string" and (requiredValue[0] is None or requiredValue[1] is None):
        exitMessageError("ERROR: Variable is without type", 56)

    if requiredValue[0] != "string" and requiredValue[1] is None and typeAndValue[0] == "TF":
        exitMessageError("ERROR: Wrong value in " + TF[typeAndValue[0]], 56)

    elif requiredValue[0] != "string" and requiredValue[1] is None and typeAndValue[0] == "LF":
        exitMessageError("ERROR: Wrong value in " + LF[typeAndValue[0]], 56)

    elif requiredValue[0] != "string" and requiredValue[1] is None and typeAndValue[0] == "GF":
        exitMessageError("ERROR: Wrong value in " + GF[typeAndValue[0]], 56)

    return requiredValue


def syntaxCheck(dictionary, inputFile):
    globalFrame = {}
    localFrame = None
    temporaryFrame = None

    frameStack = []

    valueStack = []
    callStack = []
    labels = {}
    instructionsCount = 0

    for key, value in sorted(dictionary.items()):
        if getNested(dictionary[key], 'LABEL') is not None:
            isArgsCountOk(dictionary[key]['LABEL'], 'LABEL')
            labelTypeCheck(dictionary[key]['LABEL'][1])


            if dictionary[key]['LABEL'][1][0] != 'label':
                exitMessageError("ERROR: Type is not 'label'", 32)
            else:
                labels[dictionary[key]['LABEL'][1][1]] = key
    i = 0

    key = sorted(set(dictionary))

    while i < len(key):

        if getNested(dictionary[key[i]], 'CREATEFRAME') is not None:


            isArgsCountOk(dictionary[key[i]]['CREATEFRAME'], 'CREATEFRAME')
            temporaryFrame = {}

        elif getNested(dictionary[key[i]], 'PUSHFRAME') is not None:


            isArgsCountOk(dictionary[key[i]]['PUSHFRAME'], 'PUSHFRAME')
            if temporaryFrame is None:
                exitMessageError("ERROR: Temporary frame is undefined", 55)
            else:
                frameStack.append(temporaryFrame)


                localFrame = temporaryFrame


                temporaryFrame = None

        elif getNested(dictionary[key[i]], 'POPFRAME') is not None:




            isArgsCountOk(dictionary[key[i]]['POPFRAME'], 'POPFRAME')
            if (len(frameStack)) == 0 or (frameStack is None):
                exitMessageError("ERROR: Stack of frames is empty", 55)
            if len(frameStack) != 0:
                localFrame = frameStack.pop()




            else:
                localFrame = None

        elif getNested(dictionary[key[i]], 'DEFVAR') is not None:


            isArgsCountOk(dictionary[key[i]]['DEFVAR'], 'DEFVAR')
            variable = varTypeCheck(dictionary[key[i]]['DEFVAR'][1])
            if variable[0] == "LF":
                if localFrame is None:
                    exitMessageError("ERROR: Local frame is not defined " + str(key[i]) + ". instruction", 55)
                if variable[1] in localFrame:
                    exitMessageError("ERROR: Redefinition of a variable" + variable[1], 52)
                localFrame[variable[1]] = [None, None]

            elif variable[0] == "TF":
                if temporaryFrame is None:
                    exitMessageError("ERROR: Temporary frame is not defined " + str(key[i]) + ". instruction", 55)
                if variable[1] in temporaryFrame:
                    exitMessageError("ERROR: Redefinition of a variable" + variable[1], 52)
                temporaryFrame[variable[1]] = [None, None]

            elif variable[0] == "GF":
                if variable[1] in globalFrame:
                    exitMessageError("ERROR: Redefinition of a variable" + variable[1], 52)
                globalFrame[variable[1]] = [None, None]


        elif getNested(dictionary[key[i]], 'CALL') is not None:

            isArgsCountOk(dictionary[key[i]]['CALL'], 'CALL')
            labelTypeCheck(dictionary[key[i]]['CALL'][1])
            if dictionary[key[i]]['CALL'][1][1] not in labels:
                exitMessageError("ERROR: Label is not defined.",52)

            k = labels[dictionary[key[i]]['CALL'][1][1]]-1

            callStack.append(i+2)

            i = k

        elif getNested(dictionary[key[i]], 'RETURN') is not None:
            isArgsCountOk(dictionary[key[i]]['RETURN'], 'RETURN')

            if len(callStack) == 0:
                exitMessageError("ERROR: Trying to pop empty stack for return.", 56)

            k = callStack.pop()
            i = k -1

            continue
        elif getNested(dictionary[key[i]], 'PUSHS') is not None:

            isArgsCountOk(dictionary[key[i]]['PUSHS'], 'PUSHS')

            symbol = symbCheck(dictionary[key[i]]['PUSHS'][1], temporaryFrame, localFrame, globalFrame)

            requiredValue = getValueFromVariable(symbol, temporaryFrame, localFrame, globalFrame)
            valueStack.append(requiredValue)

        elif getNested(dictionary[key[i]], 'POPS') is not None:

            isArgsCountOk(dictionary[key[i]]['POPS'], 'POPS')
            variable = varTypeCheck(dictionary[key[i]]['POPS'][1])
            isVarDefined(variable, temporaryFrame, localFrame, globalFrame)
            if len(valueStack) == 0:
                exitMessageError("ERROR: Stack of values is empty, nothing to pop.", 56)

            requiredValue = valueStack.pop()
            valueToVariable(variable, requiredValue, temporaryFrame, localFrame, globalFrame)

        elif getNested(dictionary[key[i]], 'MOVE') is not None:
            isArgsCountOk(dictionary[key[i]]['MOVE'], 'MOVE')
            variable = varTypeCheck(dictionary[key[i]]['MOVE'][1])

            isVarDefined(variable, temporaryFrame, localFrame, globalFrame)
            symbol = symbCheck(dictionary[key[i]]['MOVE'][2], temporaryFrame, localFrame, globalFrame)

            requiredValue = getValueFromVariable(symbol, temporaryFrame, localFrame, globalFrame)

            valueToVariable(variable, requiredValue, temporaryFrame, localFrame, globalFrame)

        elif getNested(dictionary[key[i]], 'ADD') is not None:

            isArgsCountOk(dictionary[key[i]]['ADD'], 'ADD')
            variable = varTypeCheck(dictionary[key[i]]['ADD'][1])
            isVarDefined(variable, temporaryFrame, localFrame, globalFrame)

            symb1 = symbCheck(dictionary[key[i]]['ADD'][2], temporaryFrame, localFrame, globalFrame)
            symb2 = symbCheck(dictionary[key[i]]['ADD'][3], temporaryFrame, localFrame, globalFrame)

            symb1 = getValueFromVariable(symb1, temporaryFrame, localFrame, globalFrame)
            symb2 = getValueFromVariable(symb2, temporaryFrame, localFrame, globalFrame)

            if symb1[0] != "int" or symb2[0] != "int":
                exitMessageError(
                    "ERROR: Need to int ADD int, but got [" + str(symb1[0]) + " ADD " + str(symb2[0]) + "]", 53)

            symb1[1] = int(symb1[1]) + int(symb2[1])
            valueToVariable(variable, symb1, temporaryFrame, localFrame, globalFrame)
        elif getNested(dictionary[key[i]], 'SUB') is not None:

            isArgsCountOk(dictionary[key[i]]['SUB'], 'SUB')
            variable = varTypeCheck(dictionary[key[i]]['SUB'][1])
            isVarDefined(variable, temporaryFrame, localFrame, globalFrame)

            symb1 = symbCheck(dictionary[key[i]]['SUB'][2], temporaryFrame, localFrame, globalFrame)
            symb2 = symbCheck(dictionary[key[i]]['SUB'][3], temporaryFrame, localFrame, globalFrame)

            symb1 = getValueFromVariable(symb1, temporaryFrame, localFrame, globalFrame)
            symb2 = getValueFromVariable(symb2, temporaryFrame, localFrame, globalFrame)

            if symb1[0] != "int" or symb2[0] != "int":
                exitMessageError(
                    "ERROR: Need to int SUB int, but got [" + str(symb1[0]) + " SUB " + str(symb2[0]) + "]", 53)

            symb1[1] = int(symb1[1]) - int(symb2[1])
            valueToVariable(variable, symb1, temporaryFrame, localFrame, globalFrame)

        elif getNested(dictionary[key[i]], 'MUL') is not None:

            isArgsCountOk(dictionary[key[i]]['MUL'], 'MUL')
            variable = varTypeCheck(dictionary[key[i]]['MUL'][1])
            isVarDefined(variable, temporaryFrame, localFrame, globalFrame)

            symb1 = symbCheck(dictionary[key[i]]['MUL'][2], temporaryFrame, localFrame, globalFrame)
            symb2 = symbCheck(dictionary[key[i]]['MUL'][3], temporaryFrame, localFrame, globalFrame)

            symb1 = getValueFromVariable(symb1, temporaryFrame, localFrame, globalFrame)
            symb2 = getValueFromVariable(symb2, temporaryFrame, localFrame, globalFrame)

            if symb1[0] != "int" or symb2[0] != "int":
                exitMessageError(
                    "ERROR: Need to int MUL int, but got [" + str(symb1[0]) + " MUL " + str(symb2[0]) + "]", 53)

            symb1[1] = int(symb1[1]) * int(symb2[1])
            valueToVariable(variable, symb1, temporaryFrame, localFrame, globalFrame)

        elif getNested(dictionary[key[i]], 'IDIV') is not None:

            isArgsCountOk(dictionary[key[i]]['IDIV'], 'IDIV')
            variable = varTypeCheck(dictionary[key[i]]['IDIV'][1])
            isVarDefined(variable, temporaryFrame, localFrame, globalFrame)

            symb1 = symbCheck(dictionary[key[i]]['IDIV'][2], temporaryFrame, localFrame, globalFrame)
            symb2 = symbCheck(dictionary[key[i]]['IDIV'][3], temporaryFrame, localFrame, globalFrame)

            symb1 = getValueFromVariable(symb1, temporaryFrame, localFrame, globalFrame)
            symb2 = getValueFromVariable(symb2, temporaryFrame, localFrame, globalFrame)

            if symb1[0] != "int" or symb2[0] != "int":
                exitMessageError(
                    "ERROR: Need to int IDIV int, but got [" + str(symb1[0]) + " IDIV " + str(symb2[0]) + "]", 53)
            if int(symb2[1]) == 0:
                exitMessageError("ERROR: IDIV by zero!", 53)
            symb1[1] = int(symb1[1]) // int(symb2[1])
            valueToVariable(variable, symb1, temporaryFrame, localFrame, globalFrame)


        elif getNested(dictionary[key[i]], 'LT') is not None:

            isArgsCountOk(dictionary[key[i]]['LT'], 'LT')
            variable = varTypeCheck(dictionary[key[i]]['LT'][1])
            isVarDefined(variable, temporaryFrame, localFrame, globalFrame)

            symb1 = symbCheck(dictionary[key[i]]['LT'][2], temporaryFrame, localFrame, globalFrame)
            symb2 = symbCheck(dictionary[key[i]]['LT'][3], temporaryFrame, localFrame, globalFrame)

            symb1 = getValueFromVariable(symb1, temporaryFrame, localFrame, globalFrame)
            symb2 = getValueFromVariable(symb2, temporaryFrame, localFrame, globalFrame)

            if symb1[0] != symb2[0]:
                exitMessageError("ERROR: You have to compare same types in LT, but got [" + str(symb1[0]) + " < "
                                 + str(symb2[0]) + "]", 53)
            if symb1[0] == "nil" or symb2[0] == "nil":
                exitMessageError("ERROR: You can't match nil with LT", 53)

            symb1[1] = symb1[1] < symb2[1]
            symb1[0] = "bool"

            valueToVariable(variable, symb1, temporaryFrame, localFrame, globalFrame)

        elif getNested(dictionary[key[i]], 'GT') is not None:

            isArgsCountOk(dictionary[key[i]]['GT'], 'GT')
            variable = varTypeCheck(dictionary[key[i]]['GT'][1])
            isVarDefined(variable, temporaryFrame, localFrame, globalFrame)

            symb1 = symbCheck(dictionary[key[i]]['GT'][2], temporaryFrame, localFrame, globalFrame)
            symb2 = symbCheck(dictionary[key[i]]['GT'][3], temporaryFrame, localFrame, globalFrame)

            symb1 = getValueFromVariable(symb1, temporaryFrame, localFrame, globalFrame)
            symb2 = getValueFromVariable(symb2, temporaryFrame, localFrame, globalFrame)

            if symb1[0] != symb2[0]:
                exitMessageError("ERROR: You have to compare same types in GT, but got [" + str(symb1[0]) + " > "
                                 + str(symb2[0]) + "]", 53)
            if symb1[0] == "nil" or symb2[0] == "nil":
                exitMessageError("ERROR: You can't match nil with GT", 53)

            symb1[1] = symb1[1] > symb2[1]
            symb1[0] = "bool"

            valueToVariable(variable, symb1, temporaryFrame, localFrame, globalFrame)

        elif getNested(dictionary[key[i]], 'EQ') is not None:

            isArgsCountOk(dictionary[key[i]]['EQ'], 'EQ')
            variable = varTypeCheck(dictionary[key[i]]['EQ'][1])
            isVarDefined(variable, temporaryFrame, localFrame, globalFrame)

            symb1 = symbCheck(dictionary[key[i]]['EQ'][2], temporaryFrame, localFrame, globalFrame)
            symb2 = symbCheck(dictionary[key[i]]['EQ'][3], temporaryFrame, localFrame, globalFrame)

            symb1 = getValueFromVariable(symb1, temporaryFrame, localFrame, globalFrame)
            symb2 = getValueFromVariable(symb2, temporaryFrame, localFrame, globalFrame)

            if symb1[0] != symb2[0] and symb1[0] != "nil" and symb2[0] != "nil":
                exitMessageError(
                    "ERROR: You have to compare same types in EQ [" + str(symb1[0]) + " EQ " + str(symb2[0]) + "]", 53)

            symb1[1] = symb1[1] == symb2[1]
            symb1[0] = "bool"

            valueToVariable(variable, symb1, temporaryFrame, localFrame, globalFrame)

        elif getNested(dictionary[key[i]], 'AND') is not None:

            isArgsCountOk(dictionary[key[i]]['AND'], 'AND')
            variable = varTypeCheck(dictionary[key[i]]['AND'][1])
            isVarDefined(variable, temporaryFrame, localFrame, globalFrame)

            symb1 = symbCheck(dictionary[key[i]]['AND'][2], temporaryFrame, localFrame, globalFrame)
            symb2 = symbCheck(dictionary[key[i]]['AND'][3], temporaryFrame, localFrame, globalFrame)

            symb1 = getValueFromVariable(symb1, temporaryFrame, localFrame, globalFrame)
            symb2 = getValueFromVariable(symb2, temporaryFrame, localFrame, globalFrame)

            if symb1[0] != "bool" or symb2[0] != "bool":
                exitMessageError(
                    "ERROR: Need to bool OR bool, but got [" + str(symb1[0]) + " AND " + str(symb2[0]) + "]", 53)

            symb1[1] = symb1[1] and symb2[1]
            symb1[0] = "bool"

            valueToVariable(variable, symb1, temporaryFrame, localFrame, globalFrame)

        elif getNested(dictionary[key[i]], 'OR') is not None:

            isArgsCountOk(dictionary[key[i]]['OR'], 'OR')
            variable = varTypeCheck(dictionary[key[i]]['OR'][1])
            isVarDefined(variable, temporaryFrame, localFrame, globalFrame)

            symb1 = symbCheck(dictionary[key[i]]['OR'][2], temporaryFrame, localFrame, globalFrame)
            symb2 = symbCheck(dictionary[key[i]]['OR'][3], temporaryFrame, localFrame, globalFrame)

            symb1 = getValueFromVariable(symb1, temporaryFrame, localFrame, globalFrame)
            symb2 = getValueFromVariable(symb2, temporaryFrame, localFrame, globalFrame)

            if symb1[0] != "bool" or symb2[0] != "bool":
                exitMessageError("ERROR: Need to bool OR bool [" + str(symb1[0]) + " OR " + str(symb2[0]) + "]", 53)

            symb1[1] = symb1[1] or symb2[1]
            symb1[0] = "bool"

            valueToVariable(variable, symb1, temporaryFrame, localFrame, globalFrame)

        elif getNested(dictionary[key[i]], 'NOT') is not None:

            isArgsCountOk(dictionary[key[i]]['NOT'], 'NOT')
            variable = varTypeCheck(dictionary[key[i]]['NOT'][1])
            isVarDefined(variable, temporaryFrame, localFrame, globalFrame)

            symb1 = symbCheck(dictionary[key[i]]['NOT'][2], temporaryFrame, localFrame, globalFrame)

            symb1 = getValueFromVariable(symb1, temporaryFrame, localFrame, globalFrame)

            if symb1[0] != "bool":
                exitMessageError("ERROR: Need to bool NOT [" + str(symb1[0]) + "]", 53)

            symb1[1] = not symb1[1]
            symb1[0] = "bool"

            valueToVariable(variable, symb1, temporaryFrame, localFrame, globalFrame)

        elif getNested(dictionary[key[i]], 'STRI2INT') is not None:

            isArgsCountOk(dictionary[key[i]]['STRI2INT'], 'STRI2INT')
            variable = varTypeCheck(dictionary[key[i]]['STRI2INT'][1])
            isVarDefined(variable, temporaryFrame, localFrame, globalFrame)

            symb1 = symbCheck(dictionary[key[i]]['STRI2INT'][2], temporaryFrame, localFrame, globalFrame)
            symb2 = symbCheck(dictionary[key[i]]['STRI2INT'][3], temporaryFrame, localFrame, globalFrame)

            symb1 = getValueFromVariable(symb1, temporaryFrame, localFrame, globalFrame)
            symb2 = getValueFromVariable(symb2, temporaryFrame, localFrame, globalFrame)

            if symb1[0] != "string" or symb2[0] != "int":
                exitMessageError("ERROR: Second argument needs to be string and third argument needs to be int.", 53)

            try:
                index = int(integer[1])
            except:
                exitMessageError("ERROR: Wrong integer in STRI2INT.", 53)

            if len(symb1[1]) <= index:
                exitMessageError("ERROR: STRI2INT: Index has to be in range of income string.", 58)

            try:
                unicodeOrd = ord(symb1[1][index])
            except:
                exitMessageError("ERROR: Wrong character in STRI2INT.", 58)

            integer[0] = "int"
            integer[1] = unicodeOrd

            valueToVariable(variable, integer, temporaryFrame, localFrame, globalFrame)

        elif getNested(dictionary[key[i]], 'INT2CHAR') is not None:

            isArgsCountOk(dictionary[key[i]]['INT2CHAR'], 'INT2CHAR')
            variable = varTypeCheck(dictionary[key[i]]['INT2CHAR'][1])
            isVarDefined(variable, temporaryFrame, localFrame, globalFrame)

            symb1 = symbCheck(dictionary[key[i]]['INT2CHAR'][2], temporaryFrame, localFrame, globalFrame)

            integer = getValueFromVariable(symb1, temporaryFrame, localFrame, globalFrame)

            if integer[0] != "int":
                exitMessageError("ERROR: INT2CHAR: Second parameter needs to be int.", 53)

            try:
                integer2Char = int(integer[1])
            except:
                exitMessageError("ERROR: Wrong integer in INT2CHAR.", 53)
            try:
                unicodeChar = chr(integer2Char)
            except:
                exitMessageError("ERROR: Wrong integer in INT2CHAR.", 58)

            integer[0] = "string"
            integer[1] = unicodeChar

            valueToVariable(variable, integer, temporaryFrame, localFrame, globalFrame)

        elif getNested(dictionary[key[i]], 'WRITE') is not None:

            isArgsCountOk(dictionary[key[i]]['WRITE'], 'WRITE')
            write = symbCheck(dictionary[key[i]]['WRITE'][1],temporaryFrame, localFrame, globalFrame)

            write = getValueFromVariable(write, temporaryFrame, localFrame, globalFrame)

            if write[0] == "bool" and write[1] is True:
                print("true", end='')
            elif write[0] == "bool" and write[1] is False:
                print("false", end='')
            elif write[0] != "nil":
                print(write[1], end='')

        elif getNested(dictionary[key[i]], 'CONCAT') is not None:

            isArgsCountOk(dictionary[key[i]]['CONCAT'], 'CONCAT')
            variable = varTypeCheck(dictionary[key[i]]['CONCAT'][1])
            isVarDefined(variable, temporaryFrame, localFrame, globalFrame)

            symb1 = symbCheck(dictionary[key[i]]['CONCAT'][2], temporaryFrame, localFrame, globalFrame)
            symb2 = symbCheck(dictionary[key[i]]['CONCAT'][3], temporaryFrame, localFrame, globalFrame)

            symb1 = getValueFromVariable(symb1, temporaryFrame, localFrame, globalFrame)
            symb2 = getValueFromVariable(symb2, temporaryFrame, localFrame, globalFrame)

            if symb1[0] != "string" or symb2[0] != "string":
                exitMessageError("ERROR: Need string CONCAT string, but got ["
                                 + str(symb1[0]) + " CONCAT " + str(symb2[0]) + "]", 53)

            symb1[1] = str(symb1[1]) + str(symb2[1])

            valueToVariable(variable, symb1, temporaryFrame, localFrame, globalFrame)

        elif getNested(dictionary[key[i]], 'STRLEN') is not None:

            isArgsCountOk(dictionary[key[i]]['STRLEN'], 'STRLEN')
            variable = varTypeCheck(dictionary[key[i]]['STRLEN'][1])
            isVarDefined(variable, temporaryFrame, localFrame, globalFrame)

            symb1 = symbCheck(dictionary[key[i]]['STRLEN'][2], temporaryFrame, localFrame, globalFrame)

            symb1 = getValueFromVariable(symb1, temporaryFrame, localFrame, globalFrame)

            if symb1[0] != "string":
                exitMessageError("ERROR: STRLEN: Need string for gain it's length, but got [" + str(symb1[0])
                                 + " CONCAT " + str(symb2[0]) + "]", 53)

            symb1[0] = "int"
            symb1[1] = len(symb1[1])

            valueToVariable(variable, symb1, temporaryFrame, localFrame, globalFrame)

        elif getNested(dictionary[key[i]], 'GETCHAR') is not None:

            isArgsCountOk(dictionary[key[i]]['GETCHAR'], 'GETCHAR')
            variable = varTypeCheck(dictionary[key[i]]['GETCHAR'][1])
            isVarDefined(variable, temporaryFrame, localFrame, globalFrame)

            symb1 = symbCheck(dictionary[key[i]]['GETCHAR'][2], temporaryFrame, localFrame, globalFrame)
            symb2 = symbCheck(dictionary[key[i]]['GETCHAR'][3], temporaryFrame, localFrame, globalFrame)

            symb1 = getValueFromVariable(symb1, temporaryFrame, localFrame, globalFrame)
            symb2 = getValueFromVariable(symb2, temporaryFrame, localFrame, globalFrame)

            if symb1[0] != "string" or symb2[0] != "int":
                exitMessageError("ERROR: GETCHAR: Second argument needs to be "
                                 "string and third argument needs to be int.", 53)

            try:
                index = int(symb2[1])
            except:
                exitMessageError("ERROR: Wrong integer in GETCHAR.", 53)

            if len(symb1[1]) <= index:
                exitMessageError("ERROR: GETCHAR: Index has to be in range of income string.", 58)

            try:
                char = symb1[1][index]
            except:
                exitMessageError("ERROR: Wrong character in GETCHAR.", 58)

            symb1[0] = "string"
            symb1[1] = char

            valueToVariable(variable, symb1, temporaryFrame, localFrame, globalFrame)

        elif getNested(dictionary[key[i]], 'SETCHAR') is not None:

            isArgsCountOk(dictionary[key[i]]['SETCHAR'], 'SETCHAR')
            variable = varTypeCheck(dictionary[key[i]]['SETCHAR'][1])
            isVarDefined(variable, temporaryFrame, localFrame, globalFrame)

            symb1 = symbCheck(dictionary[key[i]]['SETCHAR'][2], temporaryFrame, localFrame, globalFrame)
            symb2 = symbCheck(dictionary[key[i]]['SETCHAR'][3], temporaryFrame, localFrame, globalFrame)

            symb1 = getValueFromVariable(symb1, temporaryFrame, localFrame, globalFrame)
            symb2 = getValueFromVariable(symb2, temporaryFrame, localFrame, globalFrame)

            if symb1[0] != "int" or symb2[0] != "string":
                exitMessageError(
                    "ERROR: SETCHAR: Second argument needs to be string and third argument needs to be int.", 53)

            try:
                index = int(symb1[1])
            except:
                exitMessageError("ERROR: Wrong second argument in SETCHAR.", 53)

            variableValue = getValueFromVariable(variable, temporaryFrame, localFrame, globalFrame)

            if variableValue[0] != "string":
                exitMessageError("ERROR: SETCHAR: Variable to set is not string ", 53)

            if len(variableValue[1]) <= index:
                exitMessageError("ERROR: SETCHAR: Index has to be in range of income string.", 58)

            if variableValue[1] is None:
                exitMessageError("ERROR: SETCHAR: Variable to set is not string ", 58)

            if symb2[1] is None or symb2[1] == "":
                exitMessageError("ERROR: SETCHAR: Symbol is empty.", 58)

            variableValue[1][index] = symb2[1][0]

            valueToVariable(variable, variableValue, temporaryFrame, localFrame, globalFrame)

        elif getNested(dictionary[key[i]], 'TYPE') is not None:

            isArgsCountOk(dictionary[key[i]]['TYPE'], 'TYPE')
            variable = varTypeCheck(dictionary[key[i]]['TYPE'][1])
            isVarDefined(variable, temporaryFrame, localFrame, globalFrame)

            symb1 = symbCheck(dictionary[key[i]]['TYPE'][2], temporaryFrame, localFrame, globalFrame)

            try:
                symb1 = getValueFromVariable(symb1, temporaryFrame, localFrame, globalFrame)
            except:
                symb1.append("")

            type = []
            if symb1[1] == "" or symb1[0] is None:
                type.append(symb1[1])
            type.append("string")

            valueToVariable(variable, type, temporaryFrame, localFrame, globalFrame)
        elif getNested(dictionary[key[i]], 'LABEL') is not None:
            pass
        elif getNested(dictionary[key[i]], 'JUMP') is not None:

            isArgsCountOk(dictionary[key[i]]['JUMP'], 'JUMP')
            labelTypeCheck(dictionary[key[i]]['JUMP'][1])
            try:
                i = labels[dictionary[key[i]]['JUMP'][1][1]]
            except:
                exitMessageError("ERROR: Trying to jump to undefined label [" +
                                 str(dictionary[key[i]]['JUMP'][1][1]) + "]", 52)

            i = key.index(i)

            continue
        elif getNested(dictionary[key[i]], 'JUMPIFEQ') is not None:
            isArgsCountOk(dictionary[key[i]]['JUMPIFEQ'], 'JUMPIFEQ')
            labelTypeCheck(dictionary[key[i]]['JUMPIFEQ'][1])

            symb1 = symbCheck(dictionary[key[i]]['JUMPIFEQ'][2], temporaryFrame, localFrame, globalFrame)
            symb2 = symbCheck(dictionary[key[i]]['JUMPIFEQ'][3], temporaryFrame, localFrame, globalFrame)

            symb1 = getValueFromVariable(symb1, temporaryFrame, localFrame, globalFrame)
            symb2 = getValueFromVariable(symb2, temporaryFrame, localFrame, globalFrame)

            if symb1[0] != symb2[0]:
                exitMessageError(
                    "ERROR: You have to compare same types in JUMPIFEQ, but got [" + str(symb1[0]) + " and " +
                    str(symb2[0]) + "]", 53)

            if symb1[1] == symb2[1]:
                if dictionary[key[i]]['JUMPIFEQ'][1][1] not in labels:
                    exitMessageError(
                        "ERROR: Trying to jump to undefined label [" + dictionary[key[i]]['JUMPIFEQ'][1][1] + ']', 52)
                else:
                    i = labels[dictionary[key[i]]['JUMPIFEQ'][1][1]]
                    i = key.index(i)
                    continue

        elif getNested(dictionary[key[i]], 'JUMPIFNEQ') is not None:
            isArgsCountOk(dictionary[key[i]]['JUMPIFNEQ'], 'JUMPIFNEQ')
            labelTypeCheck(dictionary[key[i]]['JUMPIFNEQ'][1])

            symb1 = symbCheck(dictionary[key[i]]['JUMPIFNEQ'][2], temporaryFrame, localFrame, globalFrame)
            symb2 = symbCheck(dictionary[key[i]]['JUMPIFNEQ'][3], temporaryFrame, localFrame, globalFrame)

            symb1 = getValueFromVariable(symb1, temporaryFrame, localFrame, globalFrame)
            symb2 = getValueFromVariable(symb2, temporaryFrame, localFrame, globalFrame)

            if symb1[0] != symb2[0]:
                exitMessageError(
                    "ERROR: You have to compare same types in JUMPIFNEQ, but got [" + str(symb1[0]) + " and " + str(
                        symb2[0]) + "]", 53)
            if symb1[1] != symb2[1]:
                if dictionary[key[i]]['JUMPIFNEQ'][1][1] not in labels:
                    exitMessageError(
                        "ERROR: Trying to jump to undefined label [" + dictionary[key[i]]['JUMPIFNEQ'][1][1] + ']', 52)
                else:
                    i = labels[dictionary[key[i]]['JUMPIFNEQ'][1][1]]
                    i = key.index(i)
                    continue

        elif getNested(dictionary[key[i]], 'EXIT') is not None:
            isArgsCountOk(dictionary[key[i]]['EXIT'], 'EXIT')
            symbol = symbCheck(dictionary[key[i]]['EXIT'][1], temporaryFrame, localFrame, globalFrame)
            symbol = getValueFromVariable(symbol, temporaryFrame, localFrame, globalFrame)

            if symbol[0] != "int":
                exitMessageError("ERROR: Exit symbol has to be int.", 53)
            if 0 <= int(symbol[1]) < 50:
                sys.exit(int(symbol[1]))
            else:
                exitMessageError("ERROR: Trying to exit with number out of range 0-49", 57)

        elif getNested(dictionary[key[i]], 'DPRINT') is not None:
            isArgsCountOk(dictionary[key[i]]['DPRINT'], 'DPRINT')
            symbol = symbCheck(dictionary[key[i]]['DPRINT'][1], temporaryFrame, localFrame, globalFrame)
            symbol = getValueFromVariable(symbol, temporaryFrame, localFrame, globalFrame)

            print(symbol[1], file=sys.stderr)

        elif getNested(dictionary[key[i]], 'BREAK') is not None:
            isArgsCountOk(dictionary[key[i]]['BREAK'], 'BREAK')

            print("_________________________________________", file=sys.stderr)
            print("Current instruction: [" + str(key[i]) + ".]", file=sys.stderr)
            print("Number of done instructions: " + str(instructionsCount), file=sys.stderr)
            print("Frames:", file=sys.stderr)
            print("Local frame: {'name': ['type', value]}", file=sys.stderr)
            print(localFrame, file=sys.stderr)
            print("Temporary frame: {'name': ['type', value]}", file=sys.stderr)
            print(temporaryFrame, file=sys.stderr)
            print("Global frame: {'name': ['type', value]}", file=sys.stderr)
            print(globalFrame, file=sys.stderr)
            print("Stack frame: [{'arg1': ['type', value]}]", file=sys.stderr)
            print(frameStack, file=sys.stderr)
            print("Labels: ['name', order]", file=sys.stderr)
            print(labels, file=sys.stderr)
            print("Call stack positions: ", callStack, file=sys.stderr)
            print("_________________________________________", file=sys.stderr)

        elif getNested(dictionary[key[i]], 'READ') is not None:

            isArgsCountOk(dictionary[key[i]]['READ'], 'READ')
            variable = varTypeCheck(dictionary[key[i]]['READ'][1])
            isVarDefined(variable, temporaryFrame, localFrame, globalFrame)
            inputRead = []
            if dictionary[key[i]]['READ'][2][0] != "type":
                exitMessageError("ERROR: Expected type, got: " + str(dictionary[key[i]]['READ'][2][0]), 32)

            if not dictionary[key[i]]['READ'][2][1] in ["bool", "string", "int"]:
                exitMessageError("ERROR: Can only read bool, string or int", 32)

            typeRead = dictionary[key[i]]['READ'][2][1]

            if not (inputFile == sys.stdin):
                read = inputFile.readline()
                if read[-1:] == "\n":
                    read = read[:-1]
            else:
                try:
                    read = input()
                except:
                    read = None
            if typeRead == "bool":

                inputRead.append("bool")
                try:
                    read = read.lower()
                except:
                    inputRead.append(False)
                if read == "true":
                    inputRead.append(True)
                else:
                    inputRead.append(False)
            if typeRead == "string":
                inputRead.append("string")
                if not read:
                    inputRead.append("")
                else:
                    inputRead.append(read)
            elif typeRead == "int":
                inputRead.append("int")
                try:
                    inputRead.append(int(read))
                except:
                    inputRead.append(0)

        else:
            exitMessageError("ERROR: [" + str(key[i]) + ".]Instruction is not acceptable ", 32)

        instructionsCount = instructionsCount + 1
        i += 1


def checkAndAssignFile(file):
    try:
        openedFile = open(file, 'r')
        return openedFile
    except:
        exitMessageError("ERROR: File could not be opened!", 11)


def controlOpt(sourceFile, inputFile):
    try:
        opts, args = getopt.getopt(sys.argv[1:], '', ['source=', 'input=', 'help'])
    except getopt.GetoptError:
        exitMessageError("ERROR: Script was started with wrong combination of parameters.", 10)
    for opt, arg in opts:
        if opt == '--help':
            if len(sys.argv) == 2:
                print("HELP:\nThis program interprets code in language IPPCode19\n--help : print "
                      "helpful information about script\n--input=file : input file with XML representation IPPc"
                      "ode19 \n--source=file : source file for input()\nOne of these parameters "
                      "(--source) or (--input) need to be set. If one of them misses, script reads "
                      "necessary data from standard input")
                sys.exit(0)
            else:
                exitMessageError("ERROR: Script was started with wrong combination of parameters.", 10)
        elif opt == '--input':
            if inputFile != sys.stdin:
                exitMessageError("ERROR: Script was started with wrong combination of parameters.", 10)
            inputFile = checkAndAssignFile(arg)
            return inputFile
        elif opt == '--source':
            if sourceFile != sys.stdin:
                exitMessageError("ERROR: Script was started with wrong combination of parameters.", 10)
            sourceFile = checkAndAssignFile(arg)
            return sourceFile
        else:
            exitMessageError("Script was started with wrong combination of parameters.", 10)
    if sourceFile == sys.stdin:
        if inputFile == sys.stdin:
            exitMessageError("ERROR: There are no arguments.", 10)


def main():
    source = sys.stdin
    inputFile = sys.stdin
    file = controlOpt(source,inputFile)
    mainDictionary = parseXml(file)
    syntaxCheck(mainDictionary, file)
    sys.exit(0)


if __name__ == "__main__":
    main()
