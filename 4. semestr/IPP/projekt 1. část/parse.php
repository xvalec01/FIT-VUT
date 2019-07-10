
<?php
// kontrola na počet argumentů
if ($argc != 2 && $argc != 1)
{
    exit(10);
}
// jestliže je argument --help, vypíšu help
if ($argc == 2 && $argv[1] == '--help')
{
    print("Skript typu filtr (v jazyce PHP 7.3) načte ze standardního vstupu zdrojový kód v IPPcode19,
    zkontroluje lexikální a syntaktickou správnost kódu a vypíše na standardnívýstup XML reprezentaci 
    programu dle specifikace v sekci.\n");
    exit(0);
}
// přichystané pro rozšíření
elseif ($argc > 2 && ($argv[1] == '--help' || $argv[2] == '--help'))
{
    exit(10);
}
// proměnná pro počet instrukcí
$lineNumber = 0;

// rozdělení instrukcí podle jejich parametrů
$instructNoParam = array("CREATEFRAME","PUSHFRAME","POPFRAME","RETURN","BREAK");
$instructOneVar = array("DEFVAR","POPS");
$instructOneSymb = array("PUSHS","WRITE","EXIT","DPRINT");
$instructOneLabel = array("CALL","JUMP","LABEL");
$instructTwoVarSymb = array("MOVE","INT2CHAR","STRLEN","TYPE","NOT");
$instructTwoVarType = array("READ");
$instructThreeVarSymbSymb = array("ADD","SUB","MUL","IDIV","LT","GT","EQ","AND","OR","STRI2INT","CONCAT","GETCHAR","SETCHAR");
$instructThreeLabelSymbSymb = array("JUMPIFEQ","JUMPIFNEQ");

// regexy na správný zápis hodnot a instrukcí podle zadání
$patternLabel = '/(^([_\-$&%*!?]|[a-zA-Z]))([_\-$&%*!?]|[a-zA-Z0-9])*$/';
$patternVar = '/^(LF|TF|GF)@([_\-$&%*!?]|[a-zA-Z])([_\-$&%*!?]|[a-zA-Z0-9])*$/';
$patternSymbol = '/^(LF|TF|GF)@([_\-$&%*!?]|[a-zA-Z])([_\-$&%*!?]|[a-zA-Z0-9])*$|^(bool)@(true|false)$|^(nil@nil)$|^(int)@(.+)$|^(string)@(.*)$/';
$patternType = '/^(bool|string|int)$/';
// pro správnou kontrolu escapovaných sekvencí ve stringu
$stringCheck = '/(\\\\\D{1,3})|(\\\\\d{1,2}\D)|(\\\\\d{1,2}$)|(\\\\$)/';
// kontrola pro komentáře za .ippcode19
$languageCheck = '/^(\.ippcode19)(.?#.*)*$/';
// pro správné rozdělení při vypsiování XML
$forCheck = '/^(bool)@(true|false)$|^(nil@nil)$|^(int)@(.+)$|^(string)@(.*)$/';

// začátek pro výpis do XML
$writer = new XMLWriter();
$writer->openURI('php://output');
$writer->startDocument('1.0','UTF-8');
$writer->setIndent(42);
$writer->startElement('program');
$writer->writeAttribute('language', 'IPPcode19');

// Hlavní tělo programu, cyklus ve kterém se načítají řádky a v probíhá zde lexikální a syntaktická analýza
while (($line = fgets(STDIN)) != false)
{
    // změna jen prvního řádku (malá písmena) pro jazyk kódu
    if($lineNumber == 0)
        $line = strtolower($line);
    // odstranění zbytečných bílých znaků
    $line = trim(preg_replace('/\s+/',' ',$line));
    // kontrola správnosti zapsání jazyku kódu
    if($lineNumber == 0 && preg_match($languageCheck,$line))
    {
        $line = trim(preg_replace('/#.*/','',$line));
        $codeLine = explode(" ",$line);
        $lineNumber++;
        continue;
    }
    elseif($lineNumber == 0 && $line != '.ippcode19')
        exit(21);

    // odstranění komentářů
    $line = preg_replace('/#.*/','',$line);
    // odstranění přebytečných bílých znaků
    $line = trim(preg_replace('/\s+/',' ',$line));
    // přeskočení prázdného řádku
    if($line == '')
    {
        if($lineNumber == 0)
            exit(21);
        continue;
    }
    // rozdělení řádku na jednotlivé údaje podle mezery
    $codeLine = explode(" ",$line);

    // zvětšení písmen ve slovech s jménama
    $codeLine[0] = strtoupper($codeLine[0]);

    if(in_array($codeLine[0],$instructNoParam))
    {
        if(count($codeLine) != 1)
            exit(23);
        else
        {
            $writer->startElement('instruction');
            $writer->writeAttribute('order', $lineNumber);
            $writer->writeAttribute('opcode', $codeLine[0]);
            $writer->endElement();
        }
    }
    elseif(in_array($codeLine[0],$instructOneLabel))
    {
        if(!(preg_match($patternLabel,$codeLine[1])) || count($codeLine) != 2)
            exit(23);
        else
        {
            $writer->startElement('instruction');
            $writer->writeAttribute('order', $lineNumber);
            $writer->writeAttribute('opcode', $codeLine[0]);
            $writer->startElement('arg1');
            $writer->writeAttribute('type', "label");
            $writer->text($codeLine[1]);
            $writer->endElement();
            $writer->endElement();
        }
    }
    elseif(in_array($codeLine[0],$instructOneSymb))
    {
        if(!(preg_match($patternSymbol,$codeLine[1])) || (preg_match($stringCheck,$codeLine[1])) || count($codeLine) != 2)
        {
            exit(23);
        }
        else
        {

            $writer->startElement('instruction');
            $writer->writeAttribute('order', $lineNumber);
            $writer->writeAttribute('opcode', $codeLine[0]);
            $writer->startElement('arg1');
            if((preg_match($patternVar,$codeLine[1])))
            {
                $writer->writeAttribute('type', 'var');
                $writer->text($codeLine[1]);
            }
            elseif((preg_match($forCheck,$codeLine[1])))
            {
                $symbInfo = explode('@',$codeLine[1]);
                $writer->writeAttribute('type', $symbInfo[0]);
                $writer->text($symbInfo[1]);
            }
            $writer->endElement();
            $writer->endElement();
        }
    }
    elseif(in_array($codeLine[0],$instructOneVar))
    {
        if(!(preg_match($patternVar,$codeLine[1])) || count($codeLine) != 2)
            exit(23);
        else
        {
            $writer->startElement('instruction');
            $writer->writeAttribute('order', $lineNumber);
            $writer->writeAttribute('opcode', $codeLine[0]);
            $writer->startElement('arg1');
            $writer->writeAttribute('type', "var");
            $writer->text($codeLine[1]);
            $writer->endElement();
            $writer->endElement();
        }
    }
    elseif(in_array($codeLine[0],$instructTwoVarSymb))
    {
        if(!(preg_match($patternVar,$codeLine[1])) || !(preg_match($patternSymbol,$codeLine[2]))
            || (preg_match($stringCheck,$codeLine[2])) ||  count($codeLine) != 3)
            exit(23);
        else
        {
            $writer->startElement('instruction');
            $writer->writeAttribute('order', $lineNumber);
            $writer->writeAttribute('opcode', $codeLine[0]);
            $writer->startElement('arg1');
            $writer->writeAttribute('type', "var");
            $writer->text($codeLine[1]);
            $writer->endElement();
            $writer->startElement('arg2');
            if((preg_match($patternVar,$codeLine[2])))
            {
                $writer->writeAttribute('type', 'var');
                $writer->text($codeLine[1]);
            }
            elseif((preg_match($forCheck,$codeLine[2])))
            {
                $symbInfo = explode('@',$codeLine[2]);
                $writer->writeAttribute('type', $symbInfo[0]);
                $writer->text($symbInfo[1]);
            }
            $writer->endElement();
            $writer->endElement();
        }
    }
    elseif(in_array($codeLine[0],$instructTwoVarType))
    {
        if(!(preg_match($patternVar,$codeLine[1])) || !(preg_match($patternType,$codeLine[2])) || count($codeLine) != 3)
            exit(23);
        else
        {
            $writer->startElement('instruction');
            $writer->writeAttribute('order', $lineNumber);
            $writer->writeAttribute('opcode', $codeLine[0]);
            $writer->startElement('arg1');
            $writer->writeAttribute('type', "var");
            $writer->text($codeLine[1]);
            $writer->endElement();
            $writer->startElement('arg2');
            $writer->writeAttribute('type', 'type');
            $writer->text($codeLine[1]);
            $writer->endElement();
            $writer->endElement();
        }
    }
    elseif(in_array($codeLine[0],$instructThreeVarSymbSymb))
    {
        if(!(preg_match($patternVar,$codeLine[1]))
            || !(preg_match($patternSymbol,$codeLine[2])) || (preg_match($stringCheck,$codeLine[2]))
            || !(preg_match($patternSymbol,$codeLine[3])) || (preg_match($stringCheck,$codeLine[3]))
            || count($codeLine) != 4)
            exit(23);
        else
        {
            $writer->startElement('instruction');
            $writer->writeAttribute('order', $lineNumber);
            $writer->writeAttribute('opcode', $codeLine[0]);
            $writer->startElement('arg1');
            $writer->writeAttribute('type', "var");
            $writer->text($codeLine[1]);
            $writer->endElement();
            $writer->startElement('arg2');
            // rozdělování podle toho, co se má vypsat do XML
            if((preg_match($patternVar,$codeLine[2])))
            {
                $writer->writeAttribute('type', 'var');
                $writer->text($codeLine[2]);
            }
            elseif((preg_match($forCheck,$codeLine[2])))
            {
                $symbInfo = explode('@',$codeLine[2]);
                $writer->writeAttribute('type', $symbInfo[0]);
                $writer->text($symbInfo[1]);
            }
            $writer->endElement();
            $writer->startElement('arg3');
            if((preg_match($patternVar,$codeLine[3])))
            {
                $writer->writeAttribute('type', 'var');
                $writer->text($codeLine[3]);
            }
            elseif((preg_match($forCheck,$codeLine[3])))
            {
                $symbInfo2 = explode('@',$codeLine[3]);
                $writer->writeAttribute('type', $symbInfo2[0]);
                $writer->text($symbInfo2[1]);
            }
            $writer->endElement();
            $writer->endElement();
        }
    }
    elseif(in_array($codeLine[0],$instructThreeLabelSymbSymb))
    {
        if(!(preg_match($patternLabel,$codeLine[1]))
            || !(preg_match($patternSymbol,$codeLine[2])) || (preg_match($stringCheck,$codeLine[2]))
            || !(preg_match($patternSymbol,$codeLine[3])) || (preg_match($stringCheck,$codeLine[3]))
            || count($codeLine) != 4)
            exit(23);
        else
        {
            $writer->startElement('instruction');
            $writer->writeAttribute('order', $lineNumber);
            $writer->writeAttribute('opcode', $codeLine[0]);
            $writer->startElement('arg1');
            $writer->writeAttribute('type', "label");
            $writer->text($codeLine[1]);
            $writer->endElement();
            $writer->startElement('arg2');
            if((preg_match($patternVar,$codeLine[2])))
            {
                $writer->writeAttribute('type', 'var');
                $writer->text($codeLine[3]);
            }
            elseif((preg_match($forCheck,$codeLine[2])))
            {
                $symbInfo = explode('@',$codeLine[2]);
                $writer->writeAttribute('type', $symbInfo[0]);
                $writer->text($symbInfo[1]);
            }
            $writer->endElement();
            $writer->startElement('arg3');
            if((preg_match($patternVar,$codeLine[3])))
            {
                $writer->writeAttribute('type', 'var');
                $writer->text($codeLine[3]);
            }
            elseif((preg_match($forCheck,$codeLine[3])))
            {
                $symbInfo2 = explode('@',$codeLine[3]);
                $writer->writeAttribute('type', $symbInfo2[0]);
                $writer->text($symbInfo2[1]);
            }
            $writer->endElement();
            $writer->endElement();
        }
    }
    else
    {
        exit(22);
    }

    $lineNumber++;
}
$writer->endElement();
$writer->endDocument();
$writer->flush();