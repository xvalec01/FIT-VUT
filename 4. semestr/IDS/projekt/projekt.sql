-----------------Autoři----------------
-------------David Valecky - xvalec01
------------Martin Kladnak - xkladn00

SET serveroutput ON
DROP TABLE Zakaznik CASCADE CONSTRAINTS;
DROP TABLE Zesnuly CASCADE CONSTRAINTS;
DROP TABLE Zamestnanec CASCADE CONSTRAINTS;
DROP TABLE Hrob CASCADE CONSTRAINTS;
DROP TABLE Pohreb CASCADE CONSTRAINTS;
DROP TABLE Sektor CASCADE CONSTRAINTS;
DROP SEQUENCE idZakaznikSeq;

---------------------TABULKY---------------------
-- Vytvoření tabulek databáze
CREATE TABLE Zakaznik
(
    zakID NUMBER NOT NULL PRIMARY KEY,
    zakPrijmeni VARCHAR2(50) NOT NULL,
    zakJmeno VARCHAR2(50) NOT NULL,
    zakDatumNarozeni DATE NOT NULL,
    zakEmail VARCHAR2(255) CHECK(REGEXP_LIKE(zakEmail,'^([a-zA-Z0-9_\-\.]+)@([a-zA-Z0-9_\-\.]+)\.([a-zA-Z]{2,5})$')),
    zakTelefon VARCHAR2(15) NOT NULL
);

CREATE TABLE Zesnuly
(
    zesID NUMBER GENERATED ALWAYS AS IDENTITY(START with 1 INCREMENT by 1) PRIMARY KEY,
    zesPrijmeni VARCHAR2(50) NOT NULL,
    zesJmeno VARCHAR2(50) NOT NULL,
    zesDatumNarozeni DATE NOT NULL,
    zesDatumUmrti DATE NOT NULL,
    zesFotka BLOB NULL
);

CREATE TABLE Zamestnanec
(
    zamID NUMBER GENERATED ALWAYS AS IDENTITY(START with 1 INCREMENT by 1) PRIMARY KEY,
    zamPrijmeni VARCHAR2(50) NOT NULL,
    zamJmeno VARCHAR2(50) NOT NULL,
    zamDatumNarozeni DATE NOT NULL,
    zamEmail VARCHAR2(255) NOT NULL CHECK(REGEXP_LIKE(zamEmail,'^([a-zA-Z0-9_\-\.]+)@([a-zA-Z0-9_\-\.]+)\.([a-zA-Z]{2,5})$')),
    zamTelefon VARCHAR2(15) NOT NULL CHECK(REGEXP_LIKE(zamTelefon,'^(((00)|\+)(420|421)[0-9]{9})$|^([0-9]{9})$')),
    zamPozice VARCHAR2(50) NOT NULL
);

CREATE TABLE Sektor
(
    sekNazev VARCHAR2(1) NOT NULL,
    zamID NUMBER NOT NULL,
    CONSTRAINT sek_FK_Zamestnanec FOREIGN KEY (zamID) REFERENCES Zamestnanec(zamID),
    CONSTRAINT sekID PRIMARY KEY (sekNazev)
);


CREATE TABLE Hrob
(
    hrobID NUMBER GENERATED ALWAYS AS IDENTITY(START with 1 INCREMENT by 1) PRIMARY KEY,
    hrobTyp VARCHAR2(15) NOT NULL CHECK(hrobTyp in ('zpopelnění','kolumbárium','urnový háj','pietní loučka','hrob')),
    hrobRada NUMBER,
    hrobSloupec NUMBER,
    hrobKapacita NUMBER,
    hrobUhrazenoDo DATE,
    hrobUhrazeniPoplatku DATE,
    hrobDostupnost VARCHAR2(3) NOT NULL CHECK (hrobDostupnost='Ano' or hrobDostupnost='Ne'),
    zakID NUMBER,
    zesID NUMBER,
    sekID VARCHAR2(1) NOT NULL,
    CONSTRAINT hrob_FK_Zakaznik FOREIGN KEY (zakID) REFERENCES Zakaznik(zakID),
    CONSTRAINT hrob_FK_Zesnuly FOREIGN KEY (zesID) REFERENCES Zesnuly(zesID),
    CONSTRAINT hrob_FK_Sektor FOREIGN KEY (sekID) REFERENCES Sektor(sekNazev)
);


CREATE TABLE Pohreb
(
    pohID NUMBER GENERATED ALWAYS AS IDENTITY(START with 1 INCREMENT by 1) PRIMARY KEY,
    pohTyp VARCHAR2(20),
    pohDatumKonani DATE,
    pohMesto VARCHAR2(20),
    pohUlice VARCHAR2(20),
    pohPSC NUMBER NOT NULL CHECK(REGEXP_LIKE(pohPSC,'^[0-9]{5}$')),
    zesID NUMBER NOT NULL,
    zakID NUMBER NOT NULL,
    zamID NUMBER NOT NULL,
    CONSTRAINT poh_FK_Zesnuly FOREIGN KEY (zesID) REFERENCES Zesnuly(zesID),
    CONSTRAINT poh_FK_Zakaznik FOREIGN KEY (zakID) REFERENCES Zakaznik(zakID),
    CONSTRAINT poh_FK_Zamestnanec FOREIGN KEY (zamID) REFERENCES Zamestnanec(zamID)
);

---------------------TRIGGERS---------------------
--TRIGGER PRO INKREMENTACI IDENTIFIKATORU
CREATE SEQUENCE idZakaznikSeq START WITH 1 INCREMENT BY 1;

CREATE OR REPLACE TRIGGER pridaniZakaznika
 BEFORE INSERT ON Zakaznik
 FOR EACH ROW
BEGIN
  :NEW.zakID := idZakaznikSeq.nextval;
END pridaniZakaznika;
/

--TRIGGER PRO KONTROLU TELEFONNIHO CISLA

CREATE OR REPLACE TRIGGER KontrolaTelefonCisla
BEFORE INSERT OR UPDATE
    OF zakTelefon
    ON Zakaznik
FOR EACH ROW
DECLARE
    predvolba VARCHAR2(4);
    celeCislo Zakaznik.zakTelefon%TYPE;
BEGIN
    celeCislo := :new.zakTelefon;
    IF (LENGTH(celeCislo) > 14) or (LENGTH(celeCislo) < 13) THEN
        Raise_application_error(-20320,'Špatně zadané telefonní číslo!');
    END IF;
    IF (LENGTH(celeCislo) = 13) THEN
        predvolba := SUBSTR(celeCislo,1,4);
        IF predvolba != '+420' THEN
            IF predvolba != '+421' THEN
                Raise_application_error(-20320,'Špatně zadané telefonní číslo!');
            END IF;
            Raise_application_error(-20320,'Špatně zadané telefonní číslo!');
        END IF;
        IF (LENGTH(TRIM(TRANSLATE(celeCislo, '+0123456789',' '))) IS NOT NULL) THEN
            Raise_application_error(-20320,'Špatně zadané telefonní číslo!');
        END IF;
    END IF;
    IF (LENGTH(celeCislo) = 14) THEN
        predvolba := SUBSTR(celeCislo,1,5);
        IF predvolba != '00420' THEN
            IF predvolba != '00421' THEN
                Raise_application_error(-20320,'Špatně zadané telefonní číslo!');
            END IF;
            Raise_application_error(-20320,'Špatně zadané telefonní číslo!');
        END IF;
        IF (LENGTH(TRIM(TRANSLATE(celeCislo, '0123456789',' '))) IS NOT NULL) THEN
            Raise_application_error(-20320,'Špatně zadané telefonní číslo!');
        END IF;
    END IF;
END KontrolaTelefonCisla;
/
---------------------PROCEDURY---------------------
--PROCEDURA PRO POCET POHRBENYCH V HROBE
--Jako vstupní paramatry očekává Jméno sektoru určeného hrobu,Řadu určeného hrobu, Sloupec určného hrobu

CREATE OR REPLACE PROCEDURE Pocet_Pohrbenych_V_Hrobe(Sektor_hrob IN VARCHAR2,Rada_hrob IN NUMBER, Sloupec_hrob IN NUMBER) AS   
BEGIN
    DECLARE CURSOR pohrbeni is
    SELECT Z.zesJmeno, Z.zesPrijmeni
    FROM Hrob H, Zesnuly Z
    WHERE H.sekID = Sektor_hrob AND H.hrobRada = Rada_hrob AND H.hrobSloupec = Sloupec_hrob AND H.zesID = Z.zesID;
        jmeno_zesnuleho Zesnuly.zesJmeno%TYPE;
        prijmeni_zesnuleho Zesnuly.zesPrijmeni%TYPE;
        pocet_pohrb  NUMBER; 
    BEGIN
        pocet_pohrb := 0;
        OPEN pohrbeni;
                LOOP
                    FETCH pohrbeni INTO jmeno_zesnuleho, prijmeni_zesnuleho;
                    EXIT WHEN pohrbeni%NOTFOUND;
                    pocet_pohrb:=pocet_pohrb+1;
                    DBMS_OUTPUT.put_line('Jméno: ' || jmeno_zesnuleho || ' přijmení: ' || prijmeni_zesnuleho);
                END LOOP;
        CLOSE pohrbeni;
        
        DBMS_OUTPUT.PUT_LINE('Počet pohřbených:' || pocet_pohrb);
    END;
     EXCEPTION
            WHEN NO_DATA_FOUND THEN
            DBMS_OUTPUT.PUT_LINE('Nenalezeny žádné záznamy z tabulky Zesnuly.');
     
END;
/
--Procedura pro určení kolik pohřbů zvládl zaměstnanec za určité časové období
--Procedura na vstupu očekává ID_zaměstance, Začátek časového období, Konec časového období
CREATE OR REPLACE PROCEDURE Pocet_pohrbu_zamestnance_za_casove_obdobi(Zamestnanec_ID IN NUMBER,Odkdy IN DATE, Dokdy IN DATE) AS   
BEGIN
    DECLARE CURSOR pohrby is
    SELECT P.pohTyp, P.pohMesto, P.pohDatumKonani 
    FROM Pohreb P
    WHERE P.zamID = Zamestnanec_ID AND P.pohDatumKonani BETWEEN Odkdy AND Dokdy;
        typ_pohrbu Pohreb.pohTyp%TYPE;
        mesto Pohreb.pohMesto%TYPE;
        datum_konani Pohreb.pohDatumKonani%TYPE;
        pocet_pohrbu NUMBER;
    BEGIN
        pocet_pohrbu := 0;
        OPEN pohrby;
                LOOP
                    FETCH pohrby INTO typ_pohrbu, mesto, datum_konani;
                    EXIT WHEN pohrby%NOTFOUND;
                    pocet_pohrbu:=pocet_pohrbu+1;
                    DBMS_OUTPUT.put_line('Typ pohřbu: ' || typ_pohrbu || ' město: ' || mesto || ' datum konání: ' || datum_konani);
                END LOOP;
             
        CLOSE pohrby;
        DBMS_OUTPUT.PUT_LINE('Počet pohřbů:' || pocet_pohrbu);
    END;
END;
/
---------------------INSERTY---------------------
--Zde se nachází testovací inserty

INSERT INTO Zakaznik
(zakPrijmeni,zakJmeno,zakDatumNarozeni,zakEmail,zakTelefon)
VALUES
('Jerabek','Filp', TO_DATE('12.10.1998', 'dd.mm.yyyy'),'Jerabek@seznam.cz','+420620203203');
INSERT INTO Zakaznik
(zakPrijmeni,zakJmeno,zakDatumNarozeni,zakEmail,zakTelefon)
VALUES
('Kotásek','Filp', TO_DATE('08.11.1956', 'dd.mm.yyyy'),'Kotasek_filip@gmail.cz','+420603957648');
INSERT INTO Zakaznik
(zakPrijmeni,zakJmeno,zakDatumNarozeni,zakEmail,zakTelefon)
VALUES
('Dvořák','Martin', TO_DATE('03.04.1972', 'dd.mm.yyyy'),'Dvor_mar@email.com','+420723043112');


INSERT INTO Zesnuly
(zesJmeno,zesPrijmeni,zesDatumNarozeni,zesDatumUmrti)
VALUES
('David','Kočka',TO_DATE('26.06.1998', 'dd.mm.yyyy'), TO_DATE('01.12.2017', 'dd.mm.yyyy'));
INSERT INTO Zesnuly
(zesJmeno,zesPrijmeni,zesDatumNarozeni,zesDatumUmrti)
VALUES
('Filp','Pilný',TO_DATE('03.03.1996', 'dd.mm.yyyy'),TO_DATE('13.05.2003', 'dd.mm.yyyy'));
INSERT INTO Zesnuly
(zesJmeno,zesPrijmeni,zesDatumNarozeni,zesDatumUmrti)
VALUES
('Leonard','Pipousek',TO_DATE('11.12.1968', 'dd.mm.yyyy'),TO_DATE('04.08.2008', 'dd.mm.yyyy'));
INSERT INTO Zesnuly
(zesJmeno,zesPrijmeni,zesDatumNarozeni,zesDatumUmrti)
VALUES
('Karel','Matějka',TO_DATE('08.08.1938', 'dd.mm.yyyy'),TO_DATE('07.09.1999', 'dd.mm.yyyy'));
INSERT INTO Zesnuly
(zesJmeno,zesPrijmeni,zesDatumNarozeni,zesDatumUmrti)
VALUES
('Gábina','Urbanová',TO_DATE('07.09.1955', 'dd.mm.yyyy'),TO_DATE('11.10.1883', 'dd.mm.yyyy'));


INSERT INTO Zamestnanec
(zamPrijmeni,zamJmeno,zamDatumNarozeni,zamEmail,zamTelefon,zamPozice)
VALUES
('Valecky','David', TO_DATE('31.3.1998', 'dd.mm.yyyy'),'davidjeborec@seznam.cz','00420518329462','Kněz');
INSERT INTO Zamestnanec
(zamPrijmeni,zamJmeno,zamDatumNarozeni,zamEmail,zamTelefon,zamPozice)
VALUES
('Kladňák','Martin', TO_DATE('27.05.1998', 'dd.mm.yyyy'),'martinjes@gmail.cz','+420723702304','Hrobnik');

INSERT INTO Sektor
(sekNazev,zamID)
VALUES
('A',1);
INSERT INTO Sektor
(sekNazev,zamID)
VALUES
('B',1);

INSERT INTO Sektor
(sekNazev,zamID)
VALUES
('C',2);

INSERT INTO Hrob
(hrobTyp,hrobSloupec,hrobRada,hrobKapacita,hrobDostupnost,sekID)
VALUES
('hrob',2,5,2,'Ano','A');
INSERT INTO Hrob
(hrobTyp,hrobSloupec,hrobRada,hrobKapacita,hrobUhrazenoDo,hrobUhrazeniPoplatku,hrobDostupnost,zakID,zesID,sekID)
VALUES
('hrob',2,2,2,TO_DATE('31.12.2013', 'dd.mm.yyyy'),TO_DATE('06.01.2010', 'dd.mm.yyyy'),'Ne',1,1,'A');
INSERT INTO Hrob
(hrobTyp,hrobKapacita,hrobUhrazenoDo,hrobUhrazeniPoplatku,hrobDostupnost,zakID,zesID,sekID)
VALUES
('urnový háj',150,TO_DATE('31.12.2020', 'dd.mm.yyyy'),TO_DATE('14.01.2018', 'dd.mm.yyyy'),'Ano',2,2,'C');
INSERT INTO Hrob
(hrobTyp,hrobSloupec,hrobRada,hrobKapacita,hrobUhrazenoDo,hrobUhrazeniPoplatku,hrobDostupnost,zakID,zesID,sekID)
VALUES
('kolumbárium',10,15,1000,TO_DATE('31.03.2021', 'dd.mm.yyyy'),TO_DATE('29.03.2018', 'dd.mm.yyyy'),'Ano',3,3,'B');
INSERT INTO Hrob
(hrobTyp,hrobDostupnost,zakID,zesID,sekID)
VALUES
('pietní loučka','Ano',3,4,'C');
INSERT INTO Hrob
(hrobTyp,hrobSloupec,hrobRada,hrobKapacita,hrobUhrazenoDo,hrobUhrazeniPoplatku,hrobDostupnost,zakID,zesID,sekID)
VALUES
('hrob',2,2,2,TO_DATE('04.06.2022', 'dd.mm.yyyy'),TO_DATE('06.01.2019', 'dd.mm.yyyy'),'Ne',1,5,'A');

INSERT INTO Pohreb
(pohTyp,pohDatumKonani,pohMesto,pohUlice,pohPSC,zesID,zakID,zamID)
VALUES
('hrob',TO_DATE('09.01.2018', 'dd.mm.yyyy'),'Ratiskovice','Vitezna',69602,1,1,1);
INSERT INTO Pohreb
(pohTyp,pohDatumKonani,pohMesto,pohUlice,pohPSC,zesID,zakID,zamID)
VALUES
('urnový háj',TO_DATE('17.01.2018', 'dd.mm.yyyy'),'Brno','Cejl',60200,2,2,1);
INSERT INTO Pohreb
(pohTyp,pohDatumKonani,pohMesto,pohUlice,pohPSC,zesID,zakID,zamID)
VALUES
('kolumbárium',TO_DATE('31.03.2018', 'dd.mm.yyyy'),'Brno','Úvoz',60200,3,3,1);
INSERT INTO Pohreb
(pohTyp,pohDatumKonani,pohMesto,pohUlice,pohPSC,zesID,zakID,zamID)
VALUES
('pietní loučka',TO_DATE('09.05.2016', 'dd.mm.yyyy'),'Brno','Úvoz',60200,4,3,2);
INSERT INTO Pohreb
(pohTyp,pohDatumKonani,pohMesto,pohUlice,pohPSC,zesID,zakID,zamID)
VALUES
('hrob',TO_DATE('11.01.2019', 'dd.mm.yyyy'),'Ratiskovice','Vitezna',69602,5,1,2);

---------------------SELECTY---------------------

--Vypíše zákzaníky, kterým vypršela doba, do které mají uhradit hrob
SELECT Zakaznik.zakJmeno AS JmenoZakaznika, Zakaznik.zakPrijmeni AS PrijmeniZakaznika, hrob.hrobUhrazenoDo AS UhrazenoDo, Zakaznik.zakTelefon AS Telefon
FROM Zakaznik JOIN Hrob
ON Zakaznik.zakId = Hrob.zakID
WHERE Hrob.hrobUhrazenoDo IS NOT NULL AND Hrob.hrobUhrazenoDo <= '13.04.2019'
;
--Vypíše jména zaměstnanců, kteří vykonávali pohřeb v posledním roce a jeho typ
SELECT Zamestnanec.zamJmeno AS JmenoZamestnance, Zamestnanec.zamPrijmeni AS PrijmeniZamestnance, Pohreb.pohDatumKonani AS DatumKonani
FROM Zamestnanec JOIN Pohreb
ON Zamestnanec.zamId = Pohreb.zamId
WHERE Pohreb.pohDatumKonani >= '13.04.2018' AND Pohreb.pohDatumKonani <= '13.04.2019'
;

--Vypíše datum, kdy měl/i pohřeb zesnulí ležící v určitém hrobě
SELECT
Zesnuly.zesJmeno AS JmenoZesnuleho, Zesnuly.zesPrijmeni AS PrijmeniZesnuleho, Pohreb.pohDatumKonani AS DatumKonani
FROM Zesnuly, Hrob, Pohreb
WHERE Zesnuly.zesID = Pohreb.zesID AND Zesnuly.zesID = hrob.zesID AND Hrob.hrobRada = 2 AND Hrob.hrobSloupec = 2 AND Hrob.sekID = 'A'
;
--Vypíše počet pohřbených v hrobech typu obyčejný hrob sestupně
SELECT Hrob.hrobRada AS Řada ,Hrob.hrobSloupec AS Sloupec, Hrob.sekID AS Sektor, COUNT(*) AS PocetPohrbenych
FROM Hrob, Zesnuly
WHERE Hrob.zesId = Zesnuly.zesid AND Hrob.hrobTyp = 'hrob'
GROUP BY Hrob.hrobRada, Hrob.hrobSloupec, Hrob.sekID
ORDER BY PocetPohrbenych DESC
;
--Vypíše počet zákazníků, kteří mají nejvíc objednaných pohřbů
SELECT Zakaznik.zakPrijmeni AS Prijmeni, COUNT(*) AS Nejcastejsi_zakaznik
FROM Zakaznik,Pohreb
WHERE Zakaznik.zakID = Pohreb.zakID
GROUP BY Zakaznik.zakPrijmeni;

--Vypíše všechny volné hroby
SELECT Hrob.hrobRada AS Rada, Hrob.hrobSloupec AS Sloupec, Hrob.sekID AS Sektor
FROM Hrob
WHERE NOT EXISTS(
    SELECT Hrob.hrobId
    FROM Zakaznik
    WHERE Hrob.zakID = Zakaznik.zakID
);

--Vypíše jména zesnulých, kteří jsou v hrobech v sektoru A
SELECT Zesnuly.zesJmeno AS JmenoZesnuleho, Zesnuly.zesPrijmeni AS PrijmeniZesnuleho
FROM Zesnuly JOIN Hrob
ON Zesnuly.zesId = Hrob.zesID
WHERE Hrob.sekID IN (SELECT Hrob.sekID FROM Hrob WHERE Hrob.sekID = 'A' AND Hrob.hrobTyp = 'hrob');

---------------------EXPLAIN PLAN---------------------
EXPLAIN PLAN FOR
    SELECT Pohreb.zamID, Zamestnanec.zamJmeno, Zamestnanec.zamPrijmeni, COUNT(*) AS ObslouzenePohrby
    FROM Zamestnanec, Pohreb
    WHERE Zamestnanec.zamID = Pohreb.zamID
    GROUP BY Pohreb.zamID,Zamestnanec.zamJmeno, Zamestnanec.zamPrijmeni;

SELECT PLAN_TABLE_OUTPUT
FROM TABLE(DBMS_XPLAN.DISPLAY());

---------------------INDEX---------------------

CREATE INDEX ObslouzeniPohrbu ON Pohreb(zamID);
EXPLAIN PLAN FOR
    SELECT Pohreb.zamID, Zamestnanec.zamJmeno, Zamestnanec.zamPrijmeni, COUNT(*) AS ObslouzenePohrby
    FROM Zamestnanec, Pohreb
    WHERE Zamestnanec.zamID = Pohreb.zamID
    GROUP BY Pohreb.zamID,Zamestnanec.zamJmeno, Zamestnanec.zamPrijmeni;

SELECT PLAN_TABLE_OUTPUT
FROM TABLE(DBMS_XPLAN.DISPLAY());

DROP INDEX ObslouzeniPohrbu;
---------------------UKÁZKA PROCEDUR---------------------
EXEC Pocet_Pohrbenych_V_Hrobe('A',2,2);
EXEC Pocet_pohrbu_zamestnance_za_casove_obdobi(2,'1.1.2015', '24.12.2019');

---------------------NASTAVENÍ PRÁV XKLADN00---------------------
GRANT ALL ON Zesnuly TO xkladn00;
GRANT ALL ON Hrob TO xkladn00;
GRANT ALL ON Zakaznik TO xkladn00;
GRANT ALL ON Pohreb TO xkladn00;
GRANT ALL ON Sektor TO xkladn00;

---------------------MATERIALIZOVANÝ POHLED---------------------
DROP MATERIALIZED VIEW Hroby_zakazniku;
CREATE MATERIALIZED VIEW Hroby_zakazniku
CACHE 
BUILD IMMEDIATE
REFRESH ON COMMIT AS
    SELECT Hrob.hrobRada,Hrob.hrobSLoupec,Hrob.sekID,Zakaznik.zakID
    FROM Hrob 
    JOIN Zakaznik ON Hrob.zakID = Zakaznik.zakID;
    
GRANT ALL ON Hroby_zakazniku TO xvalec01;

---------------------UKÁZKA MATERIALIZOVANÉHO POHLEDU---------------------
INSERT INTO Hrob
(hrobTyp,hrobSloupec,hrobRada,hrobKapacita,hrobUhrazenoDo,hrobUhrazeniPoplatku,hrobDostupnost,zakID,zesID,sekID)
VALUES
('hrob',2,2,2,TO_DATE('04.06.2022', 'dd.mm.yyyy'),TO_DATE('06.01.2019', 'dd.mm.yyyy'),'Ne',2,5,'A');
SELECT * FROM Hroby_zakazniku;
COMMIT;
SELECT * FROM Hroby_zakazniku;
