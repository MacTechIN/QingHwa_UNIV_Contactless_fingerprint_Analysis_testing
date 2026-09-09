Review
Contactless Fingerprint Recognition Using Deep Learning—A
Systematic Review
AMMahmudChowdhuryandMasudulHaiderImtiaz*
DepartmentofElectricalandComputerEngineering,ClarksonUniversity,Potsdam,NY13699,USA
* Correspondence:mimtiaz@clarkson.edu
Abstract: Contactlessfingerprintidentificationsystemshavebeenintroducedtoaddressthede-
ficienciesofcontact-basedfingerprintsystems. Anumberofstudieshavebeenreportedregard-
ingcontactlessfingerprintprocessing,includingclassicalimageprocessing,themachine-learning
pipeline,andanumberofdeep-learning-basedalgorithms.Thedeep-learning-basedmethodswere
reportedtohavehigheraccuraciesthantheircounterparts.Thisstudywasthusmotivatedtopresent
asystematicreviewofthesesuccessesandthereportedlimitations.Threemethodswereresearched
forthisreview:(i)thefingerphotocapturemethodandcorrespondingimagesensors,(ii)theclassical
preprocessingmethodtoprepareafingerimageforarecognitiontask,and(iii)thedeep-learning
approachforcontactlessfingerprintrecognition.Eightscientificarticleswereidentifiedthatmatched
allinclusionandexclusioncriteria. Basedoninferencesfromthisreview,wehavediscussedhow
deeplearningmethodscouldbenefitthefieldofbiometricsandthepotentialgapsthatdeep-learning
approachesneedtoaddressforreal-worldbiometricapplications.
Keywords: biometrics; contactless fingerprint; deep learning; fingerprint analysis; fingerprint
recognition
Citation:Chowdhury,A.M.M.;
1. Introduction
Imtiaz,M.H.ContactlessFingerprint
Contactless fingerprint identification technology has the potential to be one of the
RecognitionUsingDeep
Learning—ASystematicReview.J. mostreliabletechniquesforbiometricidentification[1,2]. Thefirstcontactlessfingerprint
Cybersecur.Priv.2022,2,714–730. recognitionsystemwasintroducedin2004[3]asanalternativetotraditionalcontact-based
https://doi.org/10.3390/jcp2030036 fingerprinting [4]. Since then, interest has grown, as shown by a continually growing
numberofpublicationsbydifferentresearchgroups. Thispublicationcorroboratesthat
AcademicEditor:DandaB.Rawat
thedemandforcontactlessfingerprintrecognitionsystemsisincreasingrapidly[5]. The
Received:17July2022 NationalInstituteofStandardsandTechnology(NIST)hasalsoreportedthatcontactless
Accepted:23August2022 fingerprintrecognitionsystemisanimportantcomponentofnext-generationfingerprint
Published:8September2022 technologies [6]. Generally, a contactless fingerprint system involves a high-resolution
camera [7,8]. The captured images provide the details of fingerprints (ridge, valleys)
Publisher’sNote:MDPIstaysneutral
andwrinkles,etc.[9]. Oneofthechallengesofthetraditionalcontact-basedfingerprint
withregardtojurisdictionalclaimsin
recognitionsystemisfingerprintcapturing[10]. Duringtheacquisitionofacontact-based
publishedmapsandinstitutionalaffil-
iations. fingerprint,issuessuchasalatentfingerprintleftbyaprevioususeronthesensorsurface
leadtolowfingerprintquality[10–12]. Also,deformationanddistortionoffingerprints
occur because of the pressure on the sensor surface [12]. Distortions can be caused by
non-uniformityofthefingerpressureonthedevice, fingerridgechangesduetoheavy
Copyright: © 2022 by the authors. labororinjuries,differentilluminationonfingerskin,ormotionartifactsduringimage
Licensee MDPI, Basel, Switzerland. capturing[13]. Whenfingerprintscontactthescanner,theridgeflowmaybecomediscon-
This article is an open access article tinuous. Alotofbackgroundnoisemightalsobeintroducedduringcapture[14]. Often,
distributed under the terms and only a partial fingerprint is obtained because the rest might be either lost or smudged
conditionsoftheCreativeCommons
during capture [8], as shown in Figure 1. This process is subject to partial information,
Attribution(CCBY)license(https://
poorquality,distortions,andvariations,includingbackgroundandillumination[15]. The
creativecommons.org/licenses/by/
variationsinsensorsandtheacquisitionenvironmentmayintroduceawiderangeofintra-
4.0/).
J.Cybersecur.Priv.2022,2,714–730.https://doi.org/10.3390/jcp2030036 https://www.mdpi.com/journal/jcp

J. Cybersecur. Priv. 2022, 2, x FOR PEER REVIEW 2 of 18
J.Cybersecur.Priv.2022,2 715
variations in sensors and the acquisition environment may introduce a wide range of in-
atrnad- ainntde ri-nctlears-sclvaasrsi avbairliiatbyiilintyt hine cthape tcuarpetdurfiendg feirnpgreinrptriinntt eirnm tesromfsr eosfo rleustoiolunt,ioonri,e onrtiaetniotan-,
stieonns,o srennosiosre ,naonidses, kainndc osnkdinit icoonnsd.iAtiofinnsg. eAr pfihnogteora pcqhuoitroe dacbqyuairceodn btayc tale csosnsteancstolersds oseesnnsoort
sduofefes rnforto smufdfeerf ofrrommat dioenfoorrmlaatteionnt, ohri dladteenntfi, hngidedrperni nfitnsg[e7r,1p0r]i.nHtso [w7,1e0v]e.r H,noewwecvhear,l lneenwge cshaarle-
alelnsogepsr easreen atlhsoer ep.reFsoernetx haemrep.l eF,ocra pextuarmedpliem, caagpetsucraend biemoafgpeos ocranqu bael itoyf, pwoiothr dqiuffaelriteyn,t wsiizteh,
ldoiwfferreesnotl ustiizoen, ,lboawck rgersoouluntdiosne,g mbaecnktgartioounn,do rsuengcmoenntrtoaltlieodn,i lolurm uinncaotinotnro,allnedd fiallcuemdiinffiactuioltny,
ianndex ftarcaec tdinifgfifceualttuyr iens elixkteramctiinnugt ifaeea,tufirnegse lrikeen hmainncuetmiaeen, fti,negtce.r[ e1n6]h.aAnccceomrdenint,g ettoc. t[h1e6]N. AIScT-,
acosrtdainndga trod tfihne gNerISpTri,n at sitmanadgea,rdg efninegraerllpyritnhte ifmroangtea, lgreengeioranlloyf tthhee ffironngtearl, rreegqiuoinr eosf 5th00e dfipn-i
imagingsensorsforagood-qualityapplication[17]. Thesecanbecapturedbysmartphones
ger, requires 500 dpi imaging sensors for a good-quality application [17]. These can be
orahandheldelectronicdevice[7,18–20].
captured by smartphones or a handheld electronic device [7,18–20].
(a) (b) (c)
Figure 1. Different challenges for contact-based fingerprint images: (a) blurry images, (b) distorted
Figure1.Differentchallengesforcontact-basedfingerprintimages:(a)blurryimages,(b)distorted
image capture, (c) deformed images [21–23].
imagecapture,(c)deformedimages[21–23].
The contactless finger image obtains ridge–valley contrast that is different from a
The contactless finger image obtains ridge–valley contrast that is different from a
print made from the contact of a finger on a live-scan capture device [24]. To address this,
print made from the contact of a finger on a live-scan capture device [24]. To address
different technologies for acquiring finger photos, such as 2D and 3D fingerprints, have
this, differenttechnologiesforacquiringfingerphotos, suchas2Dand3Dfingerprints,
hbeaevne ibneternodiunctreodd [u1c3e,1d6[,2153,,2166],2. 5Im,26a]g.e Ipmraogceespsirnogc ecsasnin sgolcvaen ssoomlvee isssoumees, iwsshuieles, twheh irleestt hoef
rthesetmo frethmeaminr ienm caoinntainctcleosnst 2aDct laensds 23DD afinndge3rDprfiinntg aerreparsi n[2t7a]r.e Ians r[e2c7e]n.tI nyeraercse,n dteyeepa-rlesa,rdneienpg-
lteeachrnnionlgogtyec hhanso dloegmyohnastsradteemd osuncsctreastse idn simucacgees sreicnoigmniatgioenr, ecclaosgsnifiitcioanti,ocnl,a asnsidfi fceaattiuonre, raenpd-
freeasetunrteatrioenp r[e2s8e–n3t7a]t.i oTnhe[s2e8 –d3e7e]p.-lTeharenseindg emepo-dleealsr nhianvge malosod eblesehna evmepallosoyebde einn ceomntpalcotyleesds
ifnincgoenrptarcintlte-sbsasfiendg beiropmrinettr-ibca tseecdhnboiolomgeietrsi c[3t8e]c. hItn iosl ongecieesss[a3r8y]. tIot cisonndeucecst saa rcyomtopcroehnednuscitvae
csuomrvperye hoenn tshivee lasutersvte ryeosenatrhceh lfaitnedstinrgesse oanrc h2Dfi nadnidn g3sDo nco2nDtaacntldes3sD ficnognetrapcrtliensts rfiecnoggenrpitriionnt
rseycsotegmnist iboansesdys otenm dseebpa-sleedaronnindge teepc-hlneaorlnoginyg tote uchnndoerlostgayndto tuhonsdee mrstoadnedlst ahnodse pmoiondt eolusta tnhde
pfuotiunrteo duetvtehleopfumtuenret ddierveecltoiopnm. Ietn its duisreefcutilo tno. nIotties tuhsaetf uthletroe niso tae ptharattictuhlearre siyssatepma rotifc cualpar-
stuyrsitnemg coofnctaapcttulersisn gfincognetra pcthleostso fiimngaegreps htohtaot immiagghets itmhaptamcti gthhet ipmeprfaocrtmthaencpee roffo rtmhea ndceeepof-
tlehaerdnienegp -mleoadrneilns.g Amlsood, eitl sw. iAlll sboe, uitswefuilll btoe kunsoefwu lhtoowk nthoew lihmoiwtattihoenlsi mofi tcaltaisosnicsaol fmcalacshsiincea-l
mleaarcnhiinnge -olepaernn itnhge odpooenr ftohre ddeoepor lefoarrndinege pinl etahren cinogntainctltehses cfoinngtearcptlreisnst fiarnegae. rTphriins tstaurdeay.
Texhpislosrteudd dyeeepxp nleoureradl ndeetewponrkeu (DraNl Nne)t mweotrhkod(Ds NfoNr c)omnteatchtloedsss ffionrgecropnrtianctt rleescsogfinnitgieornp. rFionrt
rtheciso,g wniet inoene.dFeodr tthoi as,nwaleyznee etdheed mtoacahninaley lzeeatrhneinmga (cMhiLn)e-bleaaserndi naglg(oMriLth)-mbass teod caolmgopraitrhem wsittho
cDoNmNp amreetwhoitdhsD. PNhNotom ceatphtoudrse. aPnhdo tiomcaagpet purroecaensdsinimg aagree tphreo fcierssst isntgepasr efotrh ecofinrtsatcstlteespss ffionr-
cgoenrptarcintlte srescfionggneitriporni.n tWreec hoagvneit ieoxnp.lWoreedh athvee esxtepplos roefd ftehaetusrtee pesxotrfafcetaiotunr eanedxt rraecctoigonnitainodn
rbeacsoegdn oitnio MnLba asnedd oDnNMNL maentdhoDdNs.N Vmareiothuosd tse.stV oauritopuustste wstiothu ttphueitrs pweirtfhorthmeairnpcee rwfoerrme aanncae-
wlyezreed atnoa vlyazlieddatteo thvea lfiedaastiebitlhitey foefa sthibei lsituyggoefstthede sDuNggNe smteedthDodNsN. Tmhiest hpoapdes.r hTahsi sinpvaepsteir-
hgaatseidn fviensgteigrpatreindt ficanpgteurrpirnign tmceatphtoudrsin, gfinmgeerthporidnst, pfirnepgerorpcersinsitnpgr,e apnrdo cfeesastuinrge, eaxntrdacfteiaotnu rine
ebxottrha cctliaosnsiicnalb iomthagclea pssrioccaelsimsinagg eapndro mceasscihnignea nledarmnaincgh,i naes lweaerlnl iansg r,eapslawceelmlaesnrt eopfl acclaesmsiecnatl
omfectlhaosdsisc ablym deetehpo ldesarbnyindge. eAp tloetaarln oinf g32. Apatpoetrasl (owfi3th2opuatp deurspl(iwcaittihoonu) twdeurpe lfiocautniodn r)ewlateerde
ftoou thndesree tloatpeidcst.o Ftohleloswetionpgi cths.e Faoplplolwicaintigonth oefa ipnpclluicsaitoino nanodf ienxcclulussioionna cnrditeerxical,u esiigohnt cpraitpereiras,
ewigehret spealpecetresdw feorre as feulellc-tteedxtf orervaiefwul.l -textreview.

J.Cybersecur.Priv.2022,2 716
Thepaperisorganizedasfollows:first,thesystematicreviewprocedureisrepresented
inSection2alongwiththedescriptionofthreeresearchquestions(RQ).Section3presents
adetailedinvestigationoftheimage-capturingmethodusingimagesensors. Section4
exploresrelevantclassicalmethods. Section5analyzesthedeepneuralnetworkmethods
incontactlessfingerprintrecognitionsystems. Section6providesadiscussionandSection7
endswithconclusionsforfuturework.
2. ReviewMethodology
Thekeyfocusofthisreviewisanup-to-datesummaryofrecentnovelapproaches.
ThesystematicsearchprocedurewassetprimarilyfollowingthePreferredReportingItems
forSystematicReviewsandMeta-Analyses(PRISMA)[23]. Thismethodologyusedthe
followingprocesses: (a)identifyingresearchquestion(RQ),(b)sourceofstudy(c)search
strategy: settinginclusion/exclusioncriteria,(d)results.
2.1. ResearchQuestions
(1) RQ1.Howdodifferentsensorsystemscapturefingerimagestoensuretheacceptable
qualityoffingerprints?Researchfindingswillhelptoinvestigatewhetherthecapturing
systemshaveanyimpactonthemodelarchitectureortherecognitionperformance.
(2) RQ2. Howdoestheclassicalmachine-learningmethodpreprocessthecontactless
fingerimagesandprepareforrecognitionalgorithms? Researchfindingsexplorethe
classicalmethodsusedforfeatureextraction,imagesegmentation,minutiaepoint
extraction,imagedeblur,backgroundnoiseremoval,particularportionsegmentation,
andsuitablefeatureextractionfromfingerimages.
(3) RQ3. Howdodeepneuralnetworksreplacetheclassicalrecognitionmodels? The
answerwillexplorethearchitectureofrelateddeepneuralnetworksandtheirperfor-
manceimprovementovertraditionalmethods.
2.2. SourceofStudies
Thesearchforrelevantliteraturewasperformedacrosssixrepositories:GoogleScholar,
ScienceDirect,WileyOnlineLibrary,ACMDigitallibrary,MDPI,andIEEE.Searchdates
rangedfrominceptionto30April2022.
2.3. SearchStrategy
Thefollowing‘free-textsearchterms’wereused: ‘fingerphotorecognition’,‘finger-
printidentification’, ‘touchlessfingerprintrecognition’. Thesearchresultswerestrictly
restrictedtotheEnglishlanguage. Referencesfromselectedprimaryfull-textarticleswere
furtheranalyzedforrelevantpublications. Theselectionwasfurthernarrowedbyapplying
theeligibilitycriteriadescribedinTable1. Articlesfulfillingtheinclusioncriteriawere
consideredinthisreview,andthosefulfillingtheexclusioncriteriawerefilteredout.
Table1.InclusionandExclusionCriteriaforthissystematicreview.
InclusionCriteria ExclusionCriteria
Articlepublishedinpeer-reviewedvenues PapersnotwritteninEnglish
Articlepublishedsince2010 Traditionalcontact-basedfingerprintmethod
Articlesmustaddressacertaincombinationof
wordsi.e.,deeplearning+contactless
fingerprintrecognition
Automate+fingerprintidentification,3D+
contactlessidentification,smartphone/mobile
+capture,contactless+fingerphoto
Atotalof49publicationswereidentifiedthroughthedatabasesearchandthreefrom
thebibliographyofthosepublications;however,33failedtosatisfytheeligibilitycriteria

J. Cybersecur. Priv. 2022, 2, x FOR PEER REVIEW 4 of 18
A total of 49 publications were identified through the database search and three from
J. Cybersecur. Priv. 2022, 2, x FOR PtEhEeR RbEibVlIEioWg raphy of those publications; however, 33 failed to satisfy the eligib4il iotfy 1 c8 riteria
and were excluded. Thus, 16 publications ultimately fulfilled the eligibility criteria for this
J.Cybersecur.Priv.2022,2 review. Figure 2 illustrates the methodology and results of the review process. 717
A total of 49 publications were identified through the database search and three from
the bibliography of those publications; however, 33 failed to satisfy the eligibility criteria
aanndd wweerree eexxccluluddeedd. .TThhuuss, ,1166 ppuubblilcicaatitoionnss uultlitmimaatetelyly fufulflfiilllleedd tthhee eelilgigibibiliiltityy ccrrititeerriiaa ffoorr ththisis
rreevvieieww.. FFiigguurree 22 iilllluussttrraatteess tthhee mmeetthhooddoollooggyy aanndd rreessuullttss ooff tthhee rreevviieeww pprroocceessss..
Figure 2. Flow diagram depicting the systematic review strategy.
FFigiguurree 22. .FFloloww ddiaiaggrraamm ddeeppicictitningg ththee ssyysstetemmaatitcic rreevvieieww ssttrraatteeggyy..
2.4. Review Outcome
2.4. ReviewOutcome
2.4.T Rheevrieew aOreu tctowmoe types of contactless fingerprint capturing techniques: 2D and 3D.
Therearetwotypesofcontactlessfingerprintcapturingtechniques: 2Dand3D.Smart-
SmartpThhoernee sa raen tdw do igtyitpaels coafm ceornatsa cctalens st afkineg 2erDp rainntd c 3aDpt uprhinogt otse c[h1n7i,q3u9]e;s :3 D2D c oanndta c3tDle. ss fin-
phonesanddigitalcamerascantake2Dand3Dphotos[17,39];3Dcontactlessfingerprints
geSrmprairntptsh coannes a alsnod bdei gaictaqlu ciarmede rwasi tcha np htaokteo m2De tarnicd s 3tDer epoh-obtaosse [d17 c,3a9m];e 3rDas c [o3n9t]a,c 3tlDes fsi nfigne-rprint
can also be acquired with photometric stereo-based cameras [39], 3D fingerprint recon-
regcoernpsrtirnutsc tciaonn a, lssotr buec taucqreudir eldig whitt-hs cpahnontoinmge-tbraics setder e3oD-b faisnegde crapmrienrta sr [e3c9o],n 3sDtr ufinctgieornp,r ienttc . [40–
struction,structuredlight-scanning-based3Dfingerprintreconstruction,etc.[40–47]. The
reconstruction, structured light-scanning-based 3D fingerprint reconstruction, etc. [40–
47]g. eTnherea glebnioemraelt rbiciowmoerktrflioc wwoofrakfcloonwta octfl eas scofinntgaecrtplreisnst friencgogernpitrioinnt sryesctoemgniistidoensc sryibsetdemin is de-
47]. The general biometric workflow of a contactless fingerprint recognition system is de-
scrFibigeudr ein3 .Figure 3.
scribed in Figure 3.
Figure 3. Overview of the sub-systems of a generic contactless fingerprint recognition system [10].
FigFuirgeu r3e. 3O.vOevrevriveiwew oof fththee ssuubb--ssyysstteemmsso offa ag geneenreicricco cnotancttalecstslefisns gfeinrpgreinrptrreincot grnecitoiognnsityisotnem sy[s1t0e]m. [10].

J.Cybersecur.Priv.2022,2 718
J. Cybersecur. Priv. 2022, 2, x FOR PEER REVIEW 5 of 18
3. ContactlessFingerprintCapturingMethods
3.1. 2DContactlessFingerprintCapturingMethods
3. Contactless Fingerprint Capturing Methods
Duringacontactlessfingerprintcapture,oneormultiplefingersarepresentedtoan
o3p.t1i.c 2aDl dCeovnitcaectlleiskse Faincgaermpreirnat Coarplteunrisn.gT Mheetsheoddse vicescanbe(a)prototypehardwaredesigns
develDopueridngb ya croensteaacrtclehses rfsinogrer(pbr)ignte ncaeprtaulr-ep,u ornpeo osre mduelvtiipclees fcinugsetrosm ariez epdretsoenmteede ttot hane unique
noepedticsaflo drecvoicnet alicktel eas scafimnegrear oprr lienntsr. eTchoegsne idtieovnic[e4s8 c]a.n be a) prototype hardware designs
deveSlmopaerdt pbhy ornesee-abracsheedrs iomr abg) egeanceqrauli-spiutiropnosies doenveiceosf ctuhsetowmiidzeedly toa mvaeielta bthlee utenciqhunei ques to
canpeetdusr efo2rD cocnotancttalecstsl efsinsgfiernpgreinrtp rehcootgonsit[i4o]n. [T48h]e. NIST[17]publishedadocumenttoassess
Smartphone-based image acquisition is one of the widely available techniques to cap-
contactless fingerprint capturing methods; the document provides proper instructions
ture 2D contactless finger photos [4]. The NIST [17] published a document to assess con-
forcontactlessfingerprintcapturingdevices. Itdescribesthesmartphone’suniformlight
tactless fingerprint capturing methods; the document provides proper instructions for
lighting, backdrop segmentation, and motion reduction during capture. Multi-finger
contactless fingerprint capturing devices. It describes the smartphone’s uniform light
capturing techniques can also be used with smartphones [49]. The advantage of multi-
lighting, backdrop segmentation, and motion reduction during capture. Multi-finger cap-
fingercaptureistheefficiencysincefeatureextractionofallfivefingerscanoccurfrom
turing techniques can also be used with smartphones [49]. The advantage of multi-finger
ocnaepstuinreg lies tihme aegffeic[i5en0]c.yT soinecve afeluatautree tehxetrpacetrifoonr mof aanllc feivoef fNingoekrisa cNan9 5ocacnudr fHroTmC oDnee ssiinre- mobile
pghloen imeafogre t[h50is]., T1o3 2e0vafilunagteer tphrei npetsrfworemreanccaep otfu Nreodk.iaA Nfl9a5s ahnldig HhtT-Cen Daebsliered mpohboinlee pwhoanseu sedfor
apfopr rtohpisr, i1a3t2e0i flilnugmeripnraitnitosn waenred ctaoptcuorvede.r Ath felaeshnltiigrhet-fienngabelredar pehao.nHe owwase vueser,dt fhoer iamppargoe-quality
wparsiarteep iollrutmedintaotiboen paonodr ,toa scoavflear sthhlei gehnttipree rffinogrmer sawreeal. lHoonwlyeivnera, dthaer kimseatgtein qgu[a2li2ty]. wToasi mprove
imreapgoertqedu atoli tbye apnodort,o arse ad fulacsehcliagmhte praernfoorimses, wdaerllk oennlyv iirno an mdaernkt ssemttiinggh [t2p2]l.a Tyoa ivmeprryovime portant
roimlea.gAe uqtuoa-lfitoyc uansda ntod rmedauincet ecnaamnecrea onfoaises,t adnadrka rednvdirisotnamnecnetsfr ommighhta pnldayt oa pvheroyn eimm-ayalso
portant role. Auto-focus and maintenance of a standard distance from hand to phone may
be useful strategies. Figure 4 shows the identical distance and illumination from hand
also be useful strategies. Figure 4 shows the identical distance and illumination from hand
tophone.
to phone.
FiFgiguurree4 4.. HHoommooggeenneeoouus sddisitsatnacnec ielluilmluinmaitnioant iwonithw aiuthtoa fuotcousf ocacputsucrianpgt ubyri nsmgabrytpshmonaer.t phone.
FFiigguurree 55 iilllluussttrraatteess tthhee imimpprersessisoinosn osfo af fainfignegrperripnrt itnatkteank wenithw ait hcoantcaocnt-tbaacste-db afisne-dfinger-
pgrienrptrdinetv diceevi(cFei g(Fuigreur5ea 5)aa)n adndt htheec coorrrreessppoonnddiinngg fifinnggere irmimagaeg aecqauciqruedir uedsinugs ain ngona-ncoonn--contact
dteavcti cdeev(Gicea l(aGxaylaSxy8 )S.8().F (iFgiugurere5 5bb)).. TThhee cocnotnatcat-cbta-bseads efidngfienrpgreinrpt crainnt dciraenctdlyi rbeec utlsyedb feoru sed for
J. Cybersecur. Priv. 2022, 2, x FOR PEER REVIEW 6 of 18
fifninddininggf feeaattuurree ssuucchh aass: :rrididgge,e v,avlalellye,y d,edlteal tcaocreosr,e ms,imnuitniauet ieatec.,e wtch.,ewrehaes rtehaes ctohrerecsoprorneds-ponding
coinngt accotnlteascstlfiesnsg feinrpgerrinprtiinmt iamgaegwe wouoludldn neeeedda addddiittiioonnaall pprroocceesssisnign.g .
(a) (b)
Figure 5. (a) Contact-based fingerprint, (b) contactless fingerprint (Samsung Galaxy S8) [15].
Figure5.(a)Contact-basedfingerprint,(b)contactlessfingerprint(SamsungGalaxyS8)[15].
A digital camera is another tool to capture contactless 2D finger photos. The main
feature of this system is white-color- and LED-color-based image sensors [10]. A three-
camera-based system with blue LED is much more comfortable than a white LED for the
user to acquire fingerprints. The charge-coupled camera device emits a green LED, and
there is a stepper motor with a mirror that can capture five fingers at a time, making it
convenient [11].
3.2. 3D Contactless Fingerprint Capturing Methods
Researchers have used lab-developed prototypes of the 3D fingerprint capture ap-
proach, which requires (i) photometric stereo techniques, (ii) structured light scanning,
and (iii) stereo vision [51]. The photometric stereo-based 3D fingerprint method captures
multiple 2D images under heterogeneous illumination using a high-speed camera. Time-
of-flight (ToF) is the main principle of this technique, where surface reflectance is meas-
ured from fingerprint to light source [52]. This system is low-cost because it uses only one
high-speed camera and multiple LEDs. Figure 6 shows the capturing method with camera
and finger position.
Figure 6. Acquisition of 3D fingerprint using photometric stereo techniques [39].
The structured light-scanning method consists of several high-speed cameras and a
digital light-processing projector [53,54]. During the capture process, multiple 2D finger-
print images are captured under pattern illuminations, and 3D depth information is

J. Cybersecur. Priv. 2022, 2, x FOR PEER REVIEW 6 of 18
(a) (b)
J.Cybersecur.Priv.2022,2 Figure 5. (a) Contact-based fingerprint, (b) contactless fingerprint (Samsung Galaxy S8) [15]. 719
A digital camera is another tool to capture contactless 2D finger photos. The main
feature of this system is white-color- and LED-color-based image sensors [10]. A three-
Adigitalcameraisanothertooltocapturecontactless2Dfingerphotos. Themain
camera-based system with blue LED is much more comfortable than a white LED for the
featureofthissystemiswhite-color-andLED-color-basedimagesensors[10]. Athree-
user to acquire fingerprints. The charge-coupled camera device emits a green LED, and
camera-basedsystemwithblueLEDismuchmorecomfortablethanawhiteLEDforthe
there is a stepper motor with a mirror that can capture five fingers at a time, making it
usertoacquirefingerprints. Thecharge-coupledcameradeviceemitsagreenLED,and
convenient [11].
thereisasteppermotorwithamirrorthatcancapturefivefingersatatime, makingit
convenient[11].
3.2. 3D Contactless Fingerprint Capturing Methods
3.R2.es3eDarCchonetrasc thleasvseF uinsgeedr plraibn-tdCeavpetluorpinedg Mpreothtoodtyspes of the 3D fingerprint capture ap-
proach, Rwehsiecahr crheqerusirheas v(ei) upsheodtolambe-dtreicv esltoepreeod tpercohtnoitqyupeess, o(ifi)t hsteru3cDtufirendg elrigphritn stccaanpntiunrge, ap-
andp (rioiia) cshte,rweoh ivcihsiorenq [u5i1r]e.s T(hi)e pphhoottoommeettrriicc sstteerreeoo-tbeacshendi q3uDe sfi,n(giie)rsptrriunctt mureetdholidg hcatpstcuarnensi ng,
mulatnipdle( i2iiD)s itmeraegoevs iusinodne[r5 h1e].teTrhoegepnheootuoms iellturimcisntearteioon-b uassiendg3 aD hfiignhg-esrppereidn tcmametehroa.d Tciampetu-res
of-fmliguhltti p(TleoF2)D isim thaeg emsauinnd perrihnectiperloe goefn tehoius steilclhunmiqinuaet,i ownhuesrien gsuarfhaicgeh -rsepfleeecdtacnacme eirsa m. Teiams-e-of-
uredfl ifgrhotm( TfoinFg)eirsptrhientm toa ilnigphrti snocuiprlcee o[5f2th].i Tshteisc hsynsiqteume, iws lhoewre-csousrtf baceecarueflsee citt aunscees iosnmlye oansue red
highfr-ospmeefidn cgaemrperrian atntod mliguhlttipsoleu LrcEeD[s5.2 F].igTurhei s6 ssyhsotwems thise lcoawp-tcuorsintgb meceatuhsoedi twuitshe scaomnelyrao ne
andh fiignhg-esrp peeodsictiaomn.e raandmultipleLEDs. Figure6showsthecapturingmethodwithcamera
andfingerposition.
Figure6.Acquisitionof3Dfingerprintusingphotometricstereotechniques[39].
Figure 6. Acquisition of 3D fingerprint using photometric stereo techniques [39].
The structured light-scanning method consists of several high-speed cameras and
The structured light-scanning method consists of several high-speed cameras and a
adigitallight-processingprojector[53,54]. Duringthecaptureprocess,multiple2Dfin-
digital light-processing projector [53,54]. During the capture process, multiple 2D finger-
gerprint images are captured under pattern illuminations, and 3D depth information
prinist cimalcaugleast eadret hcraoputguhretdri aunngduelra tpioanttearcnco irldluinmgintoattihoensp, oainndtc o3Drr edspeponthd einnfcoersmbaettiwonee ins im-
ages[55]. Thismethodcanrecoverridge-valleydetailsandachieverelativelyaccurate3D
depthinformation. However,thehardwaresystemisexpensiveandbulky[39].
Thestereo-vision-based3Dcontactlessfingerprintmethodisusuallycomprisedof
twoormorecameras[51,56,57]. Duringthecaptureprocess, 2Dfingerprintimagesare
capturedfromdifferentviews. The3Dfingerprintsarereconstructedbycalculating3D
depthinformationbetweencorrespondingpointsaccordingtothetriangulationprinciple.
Theadvantageisthatthesystemsaresimple,low-cost,andrelativelycompact. However,
currentmethodsareusuallytime-consumingbecauseoftheextensivecomputationofthe
correspondencesbetweenpixelpoints[58]. Table2showsthe2Dand3Dcapturingdevices
andtheirapproximatecost,withlightenvironment,etc.

J. Cybersecur. Priv. 2022, 2, x FOR PEER REVIEW 7 of 18
calculated through triangulation according to the point correspondences between images
[55]. This method can recover ridge-valley details and achieve relatively accurate 3D
depth information. However, the hardware system is expensive and bulky [39].
The stereo-vision-based 3D contactless fingerprint method is usually comprised of
two or more cameras [51,56,57]. During the capture process, 2D fingerprint images are
captured from different views. The 3D fingerprints are reconstructed by calculating 3D
depth information between corresponding points according to the triangulation principle.
The advantage is that the systems are simple, low-cost, and relatively compact. However,
current methods are usually time-consuming because of the extensive computation of the
correspondences between pixel points [58]. Table 2 shows the 2D and 3D capturing de-
vices and their approximate cost, with light environment, etc.
Table 2. Overview of contactless 2D and 3D capturing devices and their properties.
J.Cybersecur.Priv.2022,2 720
Capturing Device Authors Cost Light Environment Finger Type
Mobile Phone (2D) Lee et al. [19] Low cost No extra illumination Single Finger
Table2.Overviewofcontactless2Dand3Dcapturingdevicesandtheirproperties.
Digital Camera (2D) Hiew et al. [59] Low cost Table lamp illumination Single Finger
CDaipgtiutarli nCgamDeevraic (e2D) GAeunthoovresse et al. [60] Cost Medium cost LightEnGvriereonn Lmigenhtt illumination FingFeirnTgyepr eslap
MobilePhone(2D) Leeetal.[19] Lowcost NoextDraififleluremnitn ialltuiomnination (white SingleFinger
Webcam (2D) Piuri et al. [61] Low cost Single Finger
DigitalCamera(2D) Hiewetal.[59] Lowcost Tablelampillumliginhat,t inoon light) SingleFinger
DigitalWCaembcearma( 2D) GenoKvuesmeaert aanl.d[ 6Z0h]ou [9M] ediumcosLtow cost GreenLightilluNmo iinllautmionination FingFeirnsglearp slap
SWmeabrctapmho(n2De ()2D)
Piuri
D
e
e
t
r
a
a
l
w
.[
i
6
e
1
t
]
al. [18]
Lowcost
LowD coifsfet rentilluminatioNno( iwlluhimteinliagthiot,n
Singl
F
e
i
F
n
i
g
n
e
g
r
e
s
r
lap
nolight)
Screen guidance. If flash re-
SmWaretbpchaomne (2D) KumarCanadnrZeyh oeut a[9l.] [49] Lowcost Low cost Noillumination FingFeirnsglearp slap
quired (Y/N)
Smartphone(2D) Derawietal.[18] Lowcost Noillumination Fingerslap
3 smartphones in different il- Thumb and in-
Smartphone (2D) Deb et al. [61] MediumSc rceoesnt guidance.Ifflashrequired
Smartphone(2D) Canreyetal.[49] Lowcost Fingerslap
(Y/N) lumination dex finger
3smartph2o cnaemsienradsi fwfeirtehn dtepth infor-
Smartphone(2D) Debetal.[61] Mediumcost Thumbandindexfinger
Smartphone (3D) Xie et al. [51] Medium cost illumination Finger slap
mation
Smartphone(3D) Xieetal.[51] Mediumcost 2cameraswithdepthinformation Fingerslap
4. Classical Method to Extract Features from Contactless Fingerprints
4. ClassicalMethodtoExtractFeaturesfromContactlessFingerprints
The basic steps for contactless fingerprint recognition pipeline are shown in Figure 7
as a fTlohwecbhaasrict: stepsforcontactlessfingerprintrecognitionpipelineareshowninFigure7
asaflowchart:
FFiigguurree 77.. TThhee ffuunnddaammeennttaall sstteeppss ffoorr ccoonnttaaccttlleessss ffiinnggeerrpprriinntt rreeccooggnniittiioonn ffrroomm iinnppuutt ttoo ((aa)) pprreepprroocceessss
tthhee iimmaaggeess;; ((bb)) FFeeaattuurree eexxttrraaccttiioonn ((cc)) DDiimmeennssiioonn rreedduuccttiioonn;; aanndd ((dd)) ccllaassssiififieedd tthhee ppeerrssoonn..
MMoosstt ooff tthhee ccoonnttaacctt--bbaasseedd ffiinnggeerrpprriinntt iimmaaggeess ccaappttuurreedd ffrroomm tthhee ddeevviicceess aarree ggrraayyssccaallee
aanndd rreeaaddyy ffoorrf efeaatutureree xetxrtarcatciotinon[6 [26]2.]I.n Icno ncotrnatsrta,smt, omstocsot nctoancttlaecstslefisns gfeinr-gimera-igminaggisnoglu stioolnus-
ptiroonvsi dperocvoildoer cRoGloBr iRmGaBg eimstahgaetsr tehqaut irreeqpurierpe rporceepsrsoincgesbsienfgo rbeeffeoarteu freeaetuxrtrea ecxtitoranc[t6io3n]. [T6h3]e.
pTrhiem parryimcharayll ecnhgaelsleonfgperse pofr opcreespsirnogcecsosnintagc tcleosnstfiacntgleesrsi mfinaggeesr aimndagreecso manmd ernedceodmmmeetnhdoedds
tmoeotvheordcso mtoe otvheersceocmhea ltlheensgee cshaarlelesnhgoews narien sThaobwlen3 .in Table 3.
Table 3. Overviewof challengesduringthe preprocessing ofcontactlessfinger imagesandpro-
posedapproaches.
Challenge Authors Year Approach
FingerSegmentation Wangetal.[62] 2017 HandcolorestimationinYCbCr
Rotatedpitchedprincipalorientation
Zaghettoetal.[9] 2015 Artificialneuralnetwork
estimation
Lowcontrast Wangetal.[62] 2016 CLAHEandextensions
Frequencymap,sensor-fingerdistance
Distancetothesensor,ridgelinefrequency Zaghettoetal[9] 2017
approximation
Core/principalsingularpointdetection Labatietal.[64] 2010 Poincare-basedridgeorientationanalysis
Robustthin-platesplines,deformationcorrection
Deformationcorrection Linetal.[11] 2018
model

J.Cybersecur.Priv.2022,2 721
Whenprocessedwiththeclassicalmethods,bothcontactless2Dand3Dimageshave
issueswithlowfocusofridge/valleyandblurredROI(background)[57]. Misplacedor
rotated fingers and the lack of skin deformation also cause processing issues [65]. An
imageprocessingpipelinemustbedevelopedbasedontheselectedequipmentandthe
environmentalconditionsneededduringtheimageacquisitionprocess. Imageprocessing
beginswiththefollowingcommonpreprocessingsteps:
Finger Segmentation and Detection: The initial step is to detect the finger based
oncolorandshape. Sharpness,shape,colorcontrast,andimagedepthinformationare
fourdifferentcategoriesforimprovingcontactless2Dand3Dfingerdetectionandimage
segmentation[13]. Sharpness-basedstrategiesutilizethedifferencebetweenthefocused,
blurredbackgroundandthesharpfingerarea. Thiseffectworksbestwithimagesobtained
withaverysmallfinger-to-sensordistanceandawide-openaperture. Oneexperiment
showed that the variance-modified Laplacian of Gaussian (VMLOG) algorithm is best
suitedforcontactless2Dfingerprint-capturingdevices[10,66,67]. Allfingershapes,from
thumbtolittlefinger,arehighlysimilarforallfingerpositioncodes. Amachine-learning-
basedalgorithmhasbeenappliedtoabinarizedimageintheLUVcolormodel[10]. To
make the skin color contrast and segment the skin and background color, the analysis
oftheYCbCrcolorspacerepresentsaveryvitalapproach[64,68]. Adifferentmethodof
imagesegmentationandimagedepthinformationapproachcombinedanRGBimagevia
asmartphone[69]. Thesewereabletoextractthefingerslap(thefourfingersexceptthe
thumb)frombusybackgroundsforfurtherprocessing.
Minutiae-Based Feature Extraction: One of the main conditions for pre-processed
contactlessfingerprintsisthatimagesmustbeconvertedfromRGBtogreyscale[70]. Thus,
ROI such as minutiae, ridge valley extraction, and finger orientation estimation must
also be handled with a machine-learning approach. After detecting the finger, the ROI
must be extracted, which involves the normalization of width, height, and resolution.
This3Dcontactlessfingerprintpreprocessingstageimpliesanextractedfingerimageas
input. ItshouldbenotedthatfingerdetectionandROIextractionaredoneinoutput. The
color-based segmentation of ROI extraction constrained setups depends on contactless
3Dfingergeometry[10]. Severaloperationsusedtheridge–lineorientationandshapeto
detectthecorepoint[71]. Usingasupportvectormachine(SVM)[72],itiseasytoclassify
minutiae-basedfingerprintsandtodetecttheminutiaepointsasthedetectionpointsto
refertoasacategory[73]. SVMcandeterminetheimagequalitywithfivefeaturevector
lengthssuchasgraymean,grayvariance,contrast,coherence,andthemainenergyratio.
Thesefeaturestakemuchtrainingtimetoimplement.
FingerprintImageEnhancement: Toimproveimagecontrastandsharpness,image
enhancementtechniquessuchasspatialdomaintechniquesandfrequencydomaintech-
niquescanbeusedtoimprovethequality[74,75]. Fingerimageenhancementshouldresult
inafingerprintimagewithuniformillumination. Threedifferentmethodstoachievethis
appearedintheliterature: anormalizationusingmeanandvariancefilters[30],histogram
enhancementslikecontrast-limitedadaptivehistogramequalization(CLAHE)[76],and
localbinarypatterns(LBP)forenhancingtheridge–valleycontrastofthe2Dcontactless
fingerprintsystem[77].Reducingtheblurredimagefromtheoriginalimageisanotherchal-
lengeincontactless3Dfingerprintenhancement[14]. Acombinationofimage-processing
algorithmsandmachinelearningforextractingsweatporesoffingerprintpatternslevel-3
hasbeenproposedbyGenoveseetal.[78].
5. AnalyzingtheDeepNeuralNetworksMethodsProposedfortheContactless
FingerprintRecognitionSystems
Simpleconvolutionalandpoolinglayerswereutilizedtocreatedeep-learningmodels
inmanyarticles,butamulti-taskfullyconvolutionalnetworkwasusedinthreeofthem.
Thearchitectureforthemulti-taskdeepconvolutionalnetworkisshowninFigure8.

J. Cybersecur. Priv. 2022, 2, x FOR PEER REVIEW 9 of 18
lengths such as gray mean, gray variance, contrast, coherence, and the main energy ratio.
These features take much training time to implement.
Fingerprint Image Enhancement: To improve image contrast and sharpness, image
enhancement techniques such as spatial domain techniques and frequency domain tech-
niques can be used to improve the quality [74,75]. Finger image enhancement should re-
sult in a fingerprint image with uniform illumination. Three different methods to achieve
this appeared in the literature: a normalization using mean and variance filters [30], his-
togram enhancements like contrast-limited adaptive histogram equalization (CLAHE)
[76], and local binary patterns (LBP) for enhancing the ridge–valley contrast of the 2D
contactless fingerprint system [77]. Reducing the blurred image from the original image
is another challenge in contactless 3D fingerprint enhancement [14]. A combination of im-
age-processing algorithms and machine learning for extracting sweat pores of fingerprint
patterns level-3 has been proposed by Genovese et al. [78].
5. Analyzing the Deep Neural Networks Methods Proposed for the Contactless
Fingerprint Recognition Systems
Simple convolutional and pooling layers were utilized to create deep-learning mod-
els in many articles, but a multi-task fully convolutional network was used in three of
J.Cybersecur.Priv.2022,2 them. The architecture for the multi-task deep convolutional network is shown7 2i2n Figure
8.
Figure 8. An architecture of multi-task deep convolutional networks [79].
Figure8.Anarchitectureofmulti-taskdeepconvolutionalnetworks[79].
MaMtchaticnhgin tghteh ecoconnttaaccttlleessssfi fningegreprrpinrtinwti twhaithtr aad ittrioandailticoonnatalc ct-obnasteadctfi-nbgaesrepdri nfitnugseinrgprint us-
ing ddeeeepp lleeaarrnnininggis isa an enwewdo dmoaminaiinn binio bmieotmricestrriecsse arrecshe.aIrnchor. dIner otrodreerc otgon riezceocgonntiazcet lceossntactless
fingfienrpgerrinprtsin, ttsh,itsh ipsappapere r[6[677]] ddeessccrriibbeedda ac ocnovnovluotliuotniaolnnaelu nraelunreatlw noerktw(CoNrkN ()CfrNamNe)w froarkm.ework.
The convolutional and pooling layers are the two main layers of the algorithm. The
The convolutional and pooling layers are the two main layers of the algorithm. The con-
convolutionallayerswereusedtoexecutelow-levelfeaturessuchasedges,corners,etc.
volutional layers were used to execute low-level features such as edges, corners, etc. Pool-
Poolinglayersenabledcorrectoperationssuchasreducingthedimensionoffeaturemaps.
ing lTaeyneirms aegneasbwleedre cporrorveicdte odpteortahteioCnNs Nsumcho daesl raesdaunciinnpgu tthbea tdchimfoerntsriaoinni nogf .feAattruarien imngaps. Ten
imagacecsu wraceyreo fp1r0o0vpiedrecedn ttow tahsea tCtaNinNed maftoedrefolu arsi taenra tiinopnus.tA bta9t5chpe frocern ttroafinteisntgin.g Aa ctcruarianciyn,g accu-
racy1 4o0f o1u0t0o fp2e7r5ciemnat gwesaws eartetauisnededfo arftteesrti nfogupru ripteorsaet.ions. At 95 percent of testing accuracy,
A fully convolutional network was applied for minutiae detection and extraction
140 out of 275 images were used for testing purpose.
in[79]. Theminutepointanditscorrespondingdirectionwereprocessedandanalyzed
A fully convolutional network was applied for minutiae detection and extraction in
usingcontactlessgrayscalefingerprintimagesfromtwodifferentpublicdatasets[12,80].
[79]. The minute point and its corresponding direction were processed and analyzed using
Imageswereassessedonlineafterbeingtrainedoffline. Afull-sizedcontactlessfingerprint
contfarocmtletsws ogrdaifyfesrceanlet dfiantagseertpsr(9in00t 0im, 6a0g00e)s wfraosmap tpwlioe ddiafsfearneinntp puutbanlidc ditsatcaosreretssp [o1n2d,8i0n]g. Images
werme ainsusteessgerodu onndlitnruet hafwtears binedinicga tterdaianseadn oofufltipnuet. iAn tfhuello-fsflizineedp coorntiotanc.tlIenscso fninjugnecrtipornint from
twow diitfhfearennovt edlalotassseftusn (c9ti0o0n0,,t h6i0s0m0)e twhoads acopnpcluierrde natsly alne airnnpsutht eanmdin iutsti aceordreetsepctoionndainndg minute
orientation. Oneofthemainclaimsofthisstudyisthatamulti-tasktechniqueoutperforms
ground truth was indicated as an output in the offline portion. In conjunction with a novel
anysingleminutiaedetectiontask. Anhourglass-shapedencoder–decodernetwork[81]
loss function, this method concurrently learns the minutiae detection and orientation. One
structurewasappliedforamulti-taskdeepneuralnetworkcalledContactlessMinuNet
of the main claims of this study is that a multi-task technique outperforms any single mi-
architecture[79]. Toprocesstheinputfingerprintimages,asharedencodersubnetwork
nutiwaea sduesteedc.tFioonr utpa-sska.m Apnlin hgo,tuhregsluabsns-estwhaoprkedw aesndceocdodere–ddtoeceoxdpaenrd ntehtewimorakg e[.8L1a]s stltyr,uthceture was
networksplitintotwobranchesforminutiaedetectionanddirectioncomputation.
Minutiaepointdetectionbranch: Inthisnetwork[79],theinputfeaturerepresents
thedetectionofminutiaepoints,andtheoutputrepresentstheprobabilityofeachpixel’s
minutiaepoints. Thenetworkisverysimple, witha1 × 1convolutionallayer, abatch
normalizationlayerthatstandardizestheinputlayer,andasigmoidlayer. Anon-linear
activationfunctionsigmoidlayerisusedtogenerateminutiaelocation.
Minutiaedirectionregressionbranch: Thisnetwork[79]isdesignedtopredictthe
minutiaedirection. Pixel-by-pixelimageswereextractedwithaphaseangle(cid:29)i€(0,2π). The
subnetworkworksasinputfeatures,andtheoutputlayerpredictsminutiaedirection. A
convolutionallayer(1×1),batchnormalization,andnon-linearactivationfunctiontanh
layerwereusedtopredicttheminutiaepath.
Using3×3CNNlayers,thefinalconvolutionhasbeenusedwith(stride=1)and
paddingtokeeptheheightandwidthconstant. Fortesting, theproposedmethodwas
comparedwiththebenchmarkdatasetofthePolyUdataset[12]. Theaccuracyofminutiae
detectionanditslocationincreasedto94.10%comparedto89.61%. Theproposedmethod
ofthisstudyisshowninFigure9.

J. Cybersecur. Priv. 2022, 2, x FOR PEER REVIEW 10 of 18
applied for a multi-task deep neural network called ContactlessMinuNet architecture [79].
To process the input fingerprint images, a shared encoder subnetwork was used. For up-
sampling, the subnetwork was decoded to expand the image. Lastly, the network split
into two branches for minutiae detection and direction computation.
Minutiae point detection branch: In this network [79], the input feature represents
the detection of minutiae points, and the output represents the probability of each pixel’s
minutiae points. The network is very simple, with a 1 × 1 convolutional layer, a batch
normalization layer that standardizes the input layer, and a sigmoid layer. A non-linear
activation function sigmoid layer is used to generate minutiae location.
Minutiae direction regression branch: This network [79] is designed to predict the
minutiae direction. Pixel-by-pixel images were extracted with a phase angle ѳi € (0,2π).
The subnetwork works as input features, and the output layer predicts minutiae direction.
A convolutional layer (1 × 1), batch normalization, and non-linear activation function tanh
layer were used to predict the minutiae path.
Using 3 × 3 CNN layers, the final convolution has been used with (stride = 1) and
padding to keep the height and width constant. For testing, the proposed method was
compared with the benchmark dataset of the PolyU dataset [12]. The accuracy of minutiae
J.Cybersecur.Priv.2022,2 723
detection and its location increased to 94.10% compared to 89.61%. The proposed method
of this study is shown in Figure 9.
Figure 9. Overview of the minutiae extraction algorithm for contactless fingerprints based on multi-
Figure9.Overviewoftheminutiaeextractionalgorithmforcontactlessfingerprintsbasedonmulti-
task fully deep convolutional neural network [2].
taskfullydeepconvolutionalneuralnetwork[2].
A study reported in [82] suggested how to extract a minutiae point from an input
Astudyreportedin[82]suggestedhowtoextractaminutiaepointfromaninputimage
image without preprocessing. To train the model and obtain the output without any pre-
withoutpreprocessing.Totrainthemodelandobtaintheoutputwithoutanypreprocessing,
processing, a number of deep neural networks have been deployed. Initially, JudgeNet
anumberofdeepneuralnetworkshavebeendeployed. Initially,JudgeNetwastrainedto
was trained to locate the minutiae regions and picked a general overview of detecting
locatetheminutiaeregionsandpickedageneraloverviewofdetectingminutiaepoints.
T
m
h
in
e
u
o
t
r
i
i
a
g
e
i n
p
a
o
l
i
i
n
m
ts
a
.
g
T
e
h
r
e
e
o
so
ri
l
g
u
i
t
n
io
a
n
l i
w
m
a
a
s
ge
6 4
r
0
es×olu
64
ti
0
on
an
w
d
a
5
s
0
6
0
4
p
0
p
×
i
6
w
40
it h
an
2
d
0 0
50
la
0
b
p
e
p
le
i
d
w
i
i
m
th
a
2
g
0
e
0
s.
la
A
be
m
le
a
d
x
pimooagliensg. Aw amsauxs epdootolinregd wuaces uthseedim toa greedduimcee tnhsei oimnsa,gaen ddimiteshnsoiwonesd, iamndag iet sdhiomweendsi oimnsagoef
d45im×en65si.oUnss inogf 4m5u ×l ti6s5c.a Ulesiinnpgu mtaunltdisfcoauler CinNpuNt laanyde rfso,uthr eCnNetNw olarykeprse,r ftohrem needtwvoerrky wpeerl-l
ftoorgmeetdth veearycc wureallt etoo ugtept utht.e Laactceur,raatneo othuetprudte. eLpatCeNr, Nanloatyheerr ndaemepe dCNLoNca llaNyeetr snpaemciefidc aLloly-
cinadlNiceatt espdetchieficdailrleyc tiinodniscaotfemd tinhue tdiaireewctiiothnsa omf omreincuotniaceis weiimtha ag medoirme ceonnsicoisneo imf4a5g×e d4i5maennd-
sdieocnid oefd 45th ×e 4s5p eacnifidc dleocciadteiodn t.hLea ssptleyc,iaficco lmocparteiohne.n Lsiavsetleys, taim coatmiopnreahnedndsievceis eiostnimwaetrieonm aandde
J . Cybersecur. Priv. 2022, 2, x FOR PEEdt R o eRca Eid Vsid IoEnW o r weelrime minaadtee tthoe amddin ourt iealeimloicnaattieo nth.eT mheinouvteiarev ileowcatoifonth. eThnee towvoerrkviaerwch oitfe tc 1ht 1eu o rnf e e1 it8 s-
wshoorwk naricnhFitiegcutruere1 0is. shown in Figure 10.
Figure 10. JudgeNet and LocalNet share a similar convolutional architecture [82].
Figure10.JudgeNetandLocalNetshareasimilarconvolutionalarchitecture[82].
AA mmeetthhoodd wwaass pprrooppoosseedd [[8833]] ttoo ggeett tthhee pprrooppeerr ppoossiittiioonn ooff tthhee ccoonnttaaccttlleessss ffiinnggeerrpprriinntt
ooff mmuullttii--vviieeww 33DD ffiinnggeerrpprriinntt ffeeaattuurreess uussiinngg CCNNNN.. AA ffuullllyy ccoonnvvoolluuttiioonnaall nneettwwoorrkk ((FFCCNN))
wwaass aapppplliieedd wwiitthh tthhiiss mmooddeell ffoorr aauuttoommaattiicc ffiinnggeerrpprriinntt sseeggmmeennttaattiioonn aanndd tthhrreeee SSiiaammeessee
nneettwwoorrkkss ffoorr ffiinnggeerrpprriinntt mmuullttii--vviieeww.. VVaarriioouuss ccoonnvvoolluuttiioonnaall nneeuurraall nneettwwoorrkk mmooddeellss ssuucchh
aass VVGGGG nneett [[3311]],, AAlleexxNNeett [[8844]],, aanndd GGoooogglleeNNeett [[8855]] aarree iinnttrroodduucceedd iinn tthhiiss wwoorrkk.. TThheessee
aarrcchhiitteeccttuurreess aarree vveerryy wweellll--ttrraaiinneedd ppiixxeell--ttoo--ppiixxeell ddeeeepp nneettwwoorrkkss.. FFoorr ffoorreeggrroouunndd aanndd
bbaacckkggrroouunndd sseeggmmeennttaattiioonn,, sseemmaannttiicc sseeggmmeennttaattiioonn aarrcchhiitteeccttuurree wwaass aapppplliieedd.. TThhiiss mmooddeell
cclluusstteerrss tthhee sasammee imimagaeg etotgoegtehtehre wriwthit ah daifdfeifrfeenrte cnltascsla. sTso. pTroedpircetd eiaccthe pacixhelp firxoeml f rthoem totphe-
vtoiepw-v ioefw thoef tfhinegfienrpgreirnptr,i ntht,etyh euyseuds etdhet hseosftomftamxa lxolsoss fsufnucntciotino.n T. Thhe eSSiaiammeessee ccoonnvvoolluuttiioonnaall
network worked very well to match image pairs (matched and unmatched) in the same
network. Three Siamese networks indicated the positions: top view, side 1 view, side 2
view. The network is structured with six convolutional layers with one fully connected
layer. One to five layers are followed by max-pooling, where the input patch size was 256
× 192.
The kernel size was 3 × 3 with stride value 2. The output numbers from the feature
map were 64, 96, 128, 256, and 512, generated from the 48-feature map in the first convo-
lutional layer. The final result was presented in the receiving operating characteristic
(ROC) curve and the equal error rate (EER) curve to evaluate performance. Using compu-
ting matching scores from CNN-based features and minutiae-based features, another Si-
amese convolutional neural network was applied to extract the global feature from a fin-
ger photo. They mentioned that fingerprint images have many global features that ease
extraction of the features using CNN. Using an input image size of 310 × 240, the first
convolutional layer was introduced with a kernel size of 3 × 3 with batch normalization.
By evaluating the ROC curve and EER curve, they showed the estimation of EER; minutiae
matching rate was 11.39% and 4.09%, respectively. Figure 11 shows how a fully convolu-
tional network segments the fingerprint background and directs the capturing methods
of multi-view with deep representation:
Figure 11. Automatic learning of 3D fingerprint features via deep representation [86].

J. Cybersecur. Priv. 2022, 2, x FOR PEER REVIEW 11 of 18
Figure 10. JudgeNet and LocalNet share a similar convolutional architecture [82].
A method was proposed [83] to get the proper position of the contactless fingerprint
of multi-view 3D fingerprint features using CNN. A fully convolutional network (FCN)
was applied with this model for automatic fingerprint segmentation and three Siamese
networks for fingerprint multi-view. Various convolutional neural network models such
as VGG net [31], AlexNet [84], and GoogleNet [85] are introduced in this work. These
architectures are very well-trained pixel-to-pixel deep networks. For foreground and
J.Cybersecur.Priv.2022,2 background segmentation, semantic segmentation architecture was applied. This mode 72 l 4
clusters the same image together with a different class. To predict each pixel from the top-
view of the fingerprint, they used the softmax loss function. The Siamese convolutional
network worked very well to match image pairs (matched and unmatched) in the same
networkworkedverywelltomatchimagepairs(matchedandunmatched)inthesame
network. Three Siamese networks indicated the positions: top view, side 1 view, side 2
network. ThreeSiamesenetworksindicatedthepositions: topview,side1view,side2
view. The network is structured with six convolutional layers with one fully connected
view. Thenetworkisstructuredwithsixconvolutionallayerswithonefullyconnected
layer. One to five layers are followed by max-pooling, where the input patch size was 256
layer. One to five layers are followed by max-pooling, where the input patch size was
×
2
1
5
9
6
2×.
192.
T
T
h
h
e
e
k
k
e
e
rn
rn
e
e
l
l
s
s
iz
iz
e
e
w
w
a
a
s
s
3
3
×× 3
3
w
w
it
i
h
th
st
s
r
t
i
r
d
id
e
e
v
v
al
a
u
lu
e
e
2
2
.
.
T
T
h
h
e
e
o
o
u
u
tp
tp
u
u
t
t
n
n
u
u
m
m
b
b
er
e
s
r s
fr
f
o
ro
m
m
th
th
e
e
fe
fe
a
a
tu
tu
re
re
map were 64, 96, 128, 256, and 512, generated from the 48-feature map in the first convo-
mapwere64,96,128,256,and512,generatedfromthe48-featuremapinthefirstconvolu-
lutional layer. The final result was presented in the receiving operating characteristic
tionallayer. Thefinalresultwaspresentedinthereceivingoperatingcharacteristic(ROC)
(ROC) curve and the equal error rate (EER) curve to evaluate performance. Using compu-
curve and the equal error rate (EER) curve to evaluate performance. Using computing
timnga tmchaitncghisncgo rsecsofrreosm frComN NC-NbaNs-ebdasfeeadt uferaetsuarneds amnidn umtiianeu-tbiaasee-bdafseeadt ufereast,uarneso,t hanerotShiaemr Seis-e
acmoensveo cluotniovnoalultnioeunraal lnneeutwraol rnketwwaosrakp wplaies daptopleixetdr atcot ethxtergaclot bthale fgealotubrael fferoamturaefi fnrogmer ap hfiont-o.
gTehr epyhmoteon. tTiohneeyd mtheanttifionngeedr pthriant tfiimngaegrepsrihnatv iemmagaensy hgalovbe amlfaenaytu grelosbtahla fteeaatsuereesx ttrhaactt ieoanseo f
etxhtreafcetaiotunr oesf uthsein fgeaCtuNrNes. uUssiinngg CanNiNn.p Uutsiinmga gane siinzpeuotf i3m1a0g×e s2i4z0e, othf e31fi0r s×t c2o4n0v, othluet ifoirnsat l
cloanyveorlwutaiosninatl rloadyuerc ewdaws iitnhtraodkeurcneedl wsiziteho af 3ke×rn3elw siitzhe boaft 3ch × n3o wrmitahl ibzaattciohn n.oBrymeavliazlautaiotinn.g
Btyh eevRaOluCatciunrgv tehaen RdOECE cRucruvrev aen,dth EeEyRsh couwrveed, tthheeye ssthiomwateido nthoef eEsEtiRm;amtiionnu toifa EeEmRa;t mchiinnugtiraaete
mwaatcsh1i1n.g3 9r%atea nwdas4 .1019.%39,%re aspnedc 4ti.v0e9l%y., Freigspuerect1iv1eslhy.o Fwigsuhroew 11a sfhuollwysc ohnovwo lau ftuiollnya clonnevtwolour-k
tisoengaml ennettswthoerkfi sneggemrpernintst bthaeck fginroguenrpdrainntd bdaicrkegctrsotuhnedc aapntdu rdinirgecmtse tthhoed csaopftumruinltgi- vmieewthowditsh
odf emepulrtei-pvrieeswe nwtaitthio dne:ep representation:
FFigiguurere 1111. .AAuutotommataitci clelaeranrniningg oof f33DD fifinnggeerprprirnint tfefeaatutureress vviaia ddeeeepp rerepprreesseenntatatitoionn [8[866]]. .
A CNN-based framework has been applied [78] to make an accurate comparison
betweencontactlessfingerprintsandcontact-basedfingerprints. Minutiaepoints,ridge
maps, and specific regions are the targeted metrics to establish the comparison [83]. A
multi-Siamesenetworkwasusedtotrainandlearntheminutiaefeatures. Astheimage
datasetwascollectedfromdifferentsensors,aCNN-basedcross-comparisonframework
was used to compare contactless and contact-based fingerprints. Figure 12 shows the
deep-featurerepresentationgenerationprocess:
Adataaugmentationprocesswasusedin5780contactlessandcontact-basedimages
from320fingers[78];3840imageswereusedinthetrainingsetwhiletherestwereusedfor
testingpurposes. Theimagesizeremained192×192ineveryimage. Apublicdatasetwas
usedtodeterminetheperformanceandtocompareandvalidatethedataset. Thedataset
contains1500fingersdatawith3000contactlessfingerprintsamples. Fortheperformance
metrics and evaluation, the ROC method (receiver operating characteristics) and EER
(equal error rate) were used. To obtain the fingerprint recognition, CMC (cumulative
match characteristics) and rank-one accuracy methods were applied. The comparative
experimentalevaluationsareshowninTable4. Also,inTable5showsthetotalsummary
ofthedescribedarticlesofdeeplearninginSection5.

| J. Cybersecur. Priv. 2022, 2, x FOR PEER REVIEW  |     |     |     |     |     |     |     |     |     | 12 of 18  |
| ------------------------------------------------ | --- | --- | --- | --- | --- | --- | --- | --- | --- | --------- |

A CNN-based framework has been applied [78] to make an accurate comparison be-
tween contactless fingerprints and contact-based fingerprints. Minutiae points, ridge
maps, and specific regions are the targeted metrics to establish the comparison [83]. A
multi-Siamese network was used to train and learn the minutiae features. As the image
dataset was collected from different sensors, a CNN-based cross-comparison framework
J.Cybersecur.Priv.2022,2 725
was used to compare contactless and contact-based fingerprints. Figure 12 shows the
deep-feature representation generation process:

FFigiugruer 1e21. 2D.eDepe-efepa-tfueraet urerperreespenretasteionnta gteionnergateinoner partoiocenssp urosicnegs sthuresein mgutlhtir-eSeiammuesleti n-Seitawmoreksse [n83e]t.w
orks[83].
A data augmentation process was used in 5780 contactless and contact-based images
Table4.Experimentalevaluationoftwodatasets.
from 320 fingers [78]; 3840 images were used in the training set while the rest were used
for testing purposes. The image size remained 192 × 192 in every image. A public dataset
|     |     |     |     | Experiments |     | EqualErrorRate(ERR) |     |     | Rank-OneAccuracy |     |
| --- | --- | --- | --- | ----------- | --- | ------------------- | --- | --- | ---------------- | --- |
was used to determine the performance and to compare and validate the dataset. The da-
taset cDoneftoairnmsa 1t5io0n0 cfionrgreecrtsi odnatma owditehl [38070]0o cnontactless fingerprint samples. For the perfor-
|     |     |     |     |     |     |     | 16.17% |     |     | 41.82% |
| --- | --- | --- | --- | --- | --- | --- | ------ | --- | --- | ------ |
mance metrics and edvaataluseattiAon, the ROC method (receiver operating characteristics) and
EERM (ienquutiaale emrroartc rhaetre)i nwNerIeS Tus[e8d8.] Toon odbattaaisne tthAe fingerprint r4e3c.8o3g%nition, CMC (cumula1t0iv.9e9 %
match characteristics) and rank-one accuracy methods were applied. The comparative ex-
|     |     |     | ProposedmethodondatasetA |     |     |     | 7.93% |     |     | 64.59% |
| --- | --- | --- | ------------------------ | --- | --- | --- | ----- | --- | --- | ------ |
perimental evaluations are shown in Table 4. Also, in Table 5 shows the total summary of
the deDscerfiobremd aatritoinclecso rorfe dcteioepn lmeaordneiln[g8 7in] oSnection 5.
|     |     |     |     |     |     |     | 21.60% |     |     | 38.90% |
| --- | --- | --- | --- | --- | --- | --- | ------ | --- | --- | ------ |
datasetB
Table 4. Experimental evaluation of two datasets.  MinutiaematcherinNIST[88]ondatasetB 38.01% 24.92%
|     |     |     | ProposedmethodondatasetB |              |     | Equal Error 7R.1at1e%  |     |                    |     | 58.87% |
| --- | --- | --- | ------------------------ | ------------ | --- | ---------------------- | --- | ------------------ | --- | ------ |
|     |     |     |                          | Experiments  |     |                        |     | Rank-One Accuracy  |     |        |
(ERR)
|     |     | Deformation correction model [87] on dataset A  |     |     |     | 16.17%  |     |     | 41.82%  |     |
| --- | --- | ----------------------------------------------- | --- | --- | --- | ------- | --- | --- | ------- | --- |
Table5.Thefollowingtabledescribesthesummaryoftheanalysis.
|     |     | Minutiae matcher in NIST [88] on dataset A  |     |     |     | 43.83%  |     |     | 10.99%  |     |
| --- | --- | ------------------------------------------- | --- | --- | --- | ------- | --- | --- | ------- | --- |
TrainingDPartoaposed method on dataset A  PurposeofDeep InputtoDeep 7.93%  OutputfromDeep 64.59%  Performance
| Study | Database |                                                                        |     |             |                 |         |                   |                |         |                |
| ----- | -------- | ---------------------------------------------------------------------- | --- | ----------- | --------------- | ------- | ----------------- | -------------- | ------- | -------------- |
|       |          |                                                                        |     | Learning    | NeuralNetwork   |         |                   | NeuralNetwork  |         | Metrics        |
|       |          | Deformation correction model [87] on dataset B                         |     |             |                 | 21.60%  |                   |                | 38.90%  |                |
|       |          | 275imageswith                                                          |     |             |                 |         |                   |                |         | Classification |
|       |          | Minutiae matcher iFni NngISeTrp [8ri8n] ton dataset BR GBtoGra3y8.01%  |     |             |                 |         |                   |                | 24.92%  |                |
| [67]  | Private  | 55different                                                            |     |             |                 |         | Featurematching   |                |         | (Metric        |
|       |          |                                                                        |     | Recognition | scaleimages     |         |                   |                |         |                |
|       |          | peopleProposed method on dataset B                                     |     |             |                 | 7.11%   |                   |                | 58.87%  | Accuracy)      |
|       |          | 576 0images                                                            |     |             |                 |         |                   |                |         |                |
|       |          |                                                                        |     | Minutiae    |                 |         | Extractedminutiae |                |         |                |
| [78]  | Public   |                                                                        |     |             | Grayscaleimages |         |                   |                |         | AUC,EER        |
|       |          | from320fingers                                                         |     | Extraction  |                 |         |                   | images         |         |                |
|       |          |                                                                        |     | Multiview   |                 |         |                   | Feature(Ridge, |         |                |
9000/6000/
| [79] | Private+Public |     |     | fingerprint | Grayscaleimages |     |     | valley) |     | EER |
| ---- | -------------- | --- | --- | ----------- | --------------- | --- | --- | ------- | --- | --- |
1320images
|     |     |     |     | recognition |     |     |     | representation |     |     |
| --- | --- | --- | --- | ----------- | --- | --- | --- | -------------- | --- | --- |
Classification
|      |        |           |     | Minutiae   |                 |     | ExtractedMinutiae |        |     |         |
| ---- | ------ | --------- | --- | ---------- | --------------- | --- | ----------------- | ------ | --- | ------- |
| [80] | Public | 100images |     |            | Grayscaleimages |     |                   |        |     | (Metric |
|      |        |           |     | Extraction |                 |     |                   | images |     |         |
Accuracy)
CNNbased
Similaritydistance
frameworkfor
| [82] | Private+Public | 500images |     |     | HSVimages |     |     | betweentwo |     | ROCcurve |
| ---- | -------------- | --------- | --- | --- | --------- | --- | --- | ---------- | --- | -------- |
Contactless
images
fingerprint
Tocorrect
ROCandCMC
[83] Public 9920images fingerprint Grayscaleimages Correctimages
curve
viewpoint

J.Cybersecur.Priv.2022,2 726
6. Discussion
Thisreviewisintendedtoprovideasystematicsurveyofthedeep-learningapproaches
employedforcontactlessfingerprintprocessing. Thereviewsurveyedeightfull-textsci-
entificresearcharticlesshowinghowdeeplearningcanreplacemachinelearningincon-
tactlessfingerprintcontexts. Thisreviewwasfocusedonthreemajorresearchquestions:
thecontactlessfingerphotocapturingmethod,theclassicalapproachoffingerprintrecog-
nition, and the useof deep learning. The first researchquestion shows thedirectionof
the different capturing methods and various camera sensors. This analysis might help
researcherstounderstanddivergentcapturingmethodsandtheirlimitations. Also,they
mightbemotivatedtoemploysmartphone-basedfingerphotocapturing. Inthesecond
research question, we explained how classical machine-learning techniques have been
introducedintocontactlessfingerprintrecognitionmethods. Thisreviewcoveredfeature
extraction,imagesegmentation,andblurreduction;however,systemslikedataacquisition,
datacleaning,datalabeling,etc. werebeyondthescopeofthisreview.
Themaincontributionofthispaperisthereviewoftheuseofdeeplearning,specifi-
cally,itsimpactandusabilityinthefieldofcontactlessfingerprints.Wehavediscussedhow
acontactlessrecognitionsystemcanbenefitfromusingdeeplearning. Inaddition,wehave
alsopointedoutpotentialvulnerabilitiesinclassicalmethodsandshowntheapplicability
ofdeeplearningtoreal-worldapplications. ResearchQuestion3showsthatthefollowing
factorscanimpacttheresearchofcontactlessfingerprintrecognitionsystems:
Feature learning: Deep-learning methods have an advantage over previous state-
of-the-art methods because they can learn features from data. Contactless fingerprint
recognitionsystemsrequirebothlocalandglobalfeatures[89]andarecompatiblewith
hierarchicalandstructuralfeaturelearningenabledbydeeplearning[90]. Inadditionto
processingandlabelingwiththehandcraftingdata,sometoolslikelabelmeandimage-
labelwillbedifficultinmostcases. Therefore,deeplearningcanassistinpreprocessingor
extractionofthefeaturesoffingerprintimages. Thelearnedfeaturescanbegeneralized
topreviouslyunseendatasetsandotherrelatedtasks(forexample,featureslearnedfor
contactlessfingerprintrecognitioncanalsobeusedforfingerprintattributeestimation,
e.g.,ridge,minutiaepattern). Inaddition,pre-trainingimprovesfeature-learningbylarge
amountsofunlabeleddatawhenusingsmallertrainingdatasets.
ConcentrationonIdentification: Authenticationandrecognitionhavebeenthepri-
maryfocusofdeep-learningresearchinthecontactlessfingerprintcontext. Authentication
isacomparablyeasyproblemandestimateswellforalargenumberofsubjects. However,
themorechallengingpartistheidentificationproblem. Thebiometricsystemneedsto
distinguishbetweenpotentiallymillionsofidentitiesforlarge-scaleidentification. This
systemrequirescomplexdeep-learningarchitecturestocapturedefiniteinterclassdiffer-
encesandhandlelargeintra-classvariability. Consequently,muchtrainingdatawouldbe
requiredtocapturethesevariations.
Large-scaledatasets:Thoughdeep-learningapproacheshavealreadyexceededhuman
performance on some in-the-wild, large-scale datasets, these datasets do not meet the
requirementsofreal-world,high-securityapplications. Inaddition,thereisalackoflarge-
scale datasets for contactless fingerprint modalities in biometrics to benefit from deep
learning. Even if large datasets are available, each individual needs to have sufficient
representativesamplestoconsiderforvariousinfluencingfactors.
Datasetquality: Existingfingerprintrecognitiondatasetsaremostlygatheredfromthe
publicdataset. Itisimportanttouselarge-scaledatasetsthatcapturereal-worldvariations
forbiometricstobenefitfromdeeplearning,especiallyinthecontactlessfingerprintarea.
Computingresources: Alongwiththeincreaseduseofmobiledevices,secureauthen-
tication commercial devices have become necessary modern technologies. However, if
complexdeep-learningarchitecturesarerequiredforauthentication,suchdevicesmight
nothavethenecessarycomputingresourcesforstoringthedataset. Acloud-basedsystem
couldbeasolutionforrestoringthedatacollectedfromthosedevices.

J.Cybersecur.Priv.2022,2 727
Trainingdeep-learningmodelswithpropercomputingresources: Thesuccessofdeep
learninghasbeenlargelydemonstratedbyindustrieswithaccesstolargeamountsofdata
andcomputationalresources. Formostotherresearchers,computingresourcesarelimited,
anditisimperativetospeedupthetrainingofdeep-learningapproaches. Weneedtostrive
fordata-efficientlearningalgorithms.
7. Conclusions
Thisreviewfocusedonthreemajorresearchquestions: thecontactlessfingerphoto
capturingmethod,theclassicalapproachtofingerprintrecognition,andtheuseofdeep
learning. Specifically, wehavedetaileddeep-learningmethods, asthesemethodshave
showndevelopmentincontactlessfingerprintrecognition,thoughlittlehasbeenexplored.
Theaccuracyofcontactlessfingerprintsisincreasingdaybydayandtheyhavefacilitateda
newrangeoffingerprintingapplications. Theyhaveincreasedthesecuritysystemthreats
with respect to terrorism and cyber-crime development. Commercial facilities, border
crossing areas, airports, and government access points are also employing contactless
fingerprintbiometrics. Further,creditcardaccountfraud,hijackingofwebsites,andmost
importantly,thecriticalcorruptionofgovernmentalagenciessuchastheDepartmentof
DefenseandtheDepartmentofHomelandSecurityrequirethedevelopmentofsystems
forwhichcontactlessfingerprintbiometricscanbeasolution. Thesedeep-learningmeth-
ods have demonstrated good generalization capability for different datasets. We have
summarizedtheirarchitectureandimplementationatvarioussub-stages,includingpre-
processing, features extraction, classification, or matching. This study also covered the
possibledrawbacksofdeep-learningmodels.
Insummary,deep-learning-basedcontactless3Dfingerprintidentificationsystems
haveshownenhancedusability,andsoontheywillbeawidelyusedbiometricperformance
modality. Therefore, our future research will be focused on creating new or existing
deep-learningtechniquestoaddresscertainupcomingcontactlessfingerprintchallenges,
suchasspeedingupfeatureextraction,reducingtheamountoftimerequiredtoprocess
images,andimprovingidentificationaccuracy. Additionally,otherbiometriccharacteristics
suchaspatternsinpalmprintswillbetakenintoconsiderationasapplicationsofdeep-
learningtechniques.
AuthorContributions:A.M.M.C.preparedthearticleandM.H.I.Supervisedhim.Allauthorshave
readandagreedtothepublishedversionofthemanuscript.
Funding:Thisresearchreceivednoexternalfunding.
InstitutionalReviewBoardStatement:Notapplicable.
InformedConsentStatement:Notapplicable.
DataAvailabilityStatement:Notapplicable.
ConflictsofInterest:Theauthorsdeclarethatnoconflictofinterest.
References
1. Maltoni,D.;Maio,D.;Jain,A.K.;Prabhakar,S.Syntheticfingerprintgeneration.InHandbookofFingerprintRecognition;Springer:
London,UK,2009;pp.271–302.
2. Choi,H.;Choi,K.;Kim,J.Mosaicingtouchlessandmirror-reflectedfingerprintimages.IEEETrans.Inf.ForensicsSecur.2010,5,
52–61.[CrossRef]
3. Song, Y.; Lee, C.; Kim, J.Anewschemefortouchlessfingerprintrecognitionsystem. InProceedingsof2004International
SymposiumonIntelligentSignalProcessingandCommunicationSystems,ISPACS,Seoul,Korea,18–19November2004.
4. Kumar, A. Introduction to trends in fingerprint identification. In Contactless 3D Fingerprint Identification; Springer:
Berlin/Heidelberg,Germany,2018;pp.1–15.
5. Oduah,U.I.;Kevin,I.F.;Oluwole,D.O.;Izunobi,J.U.Towardsahigh-precisioncontactlessfingerprintscannerforbiometric
authentication.Array2021,11,100083.[CrossRef][PubMed]
6. Stanton,B.C.;Stanton,B.C.;Theofanos,M.F.;Furman,S.M.;Grother,P.J.UsabilityTestingofaContactlessFingerprintDevice:Part2;
USDepartmentofCommerce,NationalInstituteofStandardsandTechnology:Gaithersburg,MD,USA,2016.

J.Cybersecur.Priv.2022,2 728
7. Raghavendra,R.;Busch,C.;Yang,B.Scaling-robustfingerprintverificationwithsmartphonecamerainreal-lifescenarios.In
Proceedingsofthe2013IEEESixthInternationalConferenceonBiometrics:Theory,ApplicationsandSystems(BTAS),Arlington,
VA,USA,29September–2October2013.
8. Mil’shtein,S.;Paradise,M.;Bustos,P.;Baier,M.;Foret,S.;Kunnil,V.O.;Northrup,J.Contactlesschallenges.Biom.Technol.Today
2011,2011,10–11.[CrossRef]
9. Kumar,A.; Zhou,Y.Contactlessfingerprintidentificationusinglevelzerofeatures. InProceedingsoftheIEEECVPR2011
Workshops,ColoradoSprings,CO,USA,20–25June2011.
10. Priesnitz,J.;Rathgeb,C.;Buchmann,N.;Busch,C.;Margraf,M.Anoverviewoftouchless2Dfingerprintrecognition.EURASIPJ.
ImageVideoProcess.2021,2021,1–28.[CrossRef]
11. Noh,D.;Choi,H.;Kim,J.Touchlesssensorcapturingfivefingerprintimagesbyonerotatingcamera.Opt.Eng.2011,50,113202.
[CrossRef]
12. Lin,C.;Kumar,A.Matchingcontactlessandcontact-basedconventionalfingerprintimagesforbiometricsidentification.IEEE
Trans.ImageProcess.2018,27,2008–2021.[CrossRef]
13. Wang,Y.;Hassebrook,L.G.;Lau,D.L.Dataacquisitionandprocessingof3-Dfingerprints.IEEETrans.Inf.ForensicsSecur.2010,5,
750–760.[CrossRef]
14. Tang, Y.; Jiang, L.; Hou, Y.; Wang, R. Contactless fingerprint image enhancement algorithm based on Hessian matrix and
STFT.InProceedingsofthe20172ndInternationalConferenceonMultimediaandImageProcessing(ICMIP),Wuhan,China,
17–19March2017.
15. Dharavath,K.;Talukdar,F.A.;Laskar,R.H.Studyonbiometricauthenticationsystems,challengesandfuturetrends:Areview.In
Proceedingsofthe2013IEEEInternationalConferenceonComputationalIntelligenceandComputingResearch,Enathi,India,
26–28December2013.
16. Parziale,G.;Chen,Y.Advancedtechnologiesfortouchlessfingerprintrecognition.InHandbookofRemoteBiometrics;Springer:
London,UK,2009;pp.83–109.
17. Libert, J.; Grantham, J.; Bandini, B.; Wood, S.; Garris, M.; Ko, K.; Byers, F.; Watson, C.Guidanceforevaluatingcontactless
fingerprintacquisitiondevices.NISTSpec.Publ.2018,500,305.
18. Derawi, M.O.; Yang, B.; Busch, C.Fingerprintrecognitionwithembeddedcamerasonmobilephones. InProceedingsofthe
InternationalConferenceonSecurityandPrivacyinMobileInformationandCommunicationSystems;Springer: Berlin/Heidelberg,
Germany,2011.
19. Lee,C.;Lee,S.;Kim,J.;Kim,S.J.Preprocessingofafingerprintimagecapturedwithamobilecamera.InInternationalConference
onBiometrics;Springer:Berlin/Heidelberg,Germany,2006.
20. Su,Q.;Tian,J.;Chen,X.;Yang,X.Afingerprintauthenticationsystembasedonmobilephone. InInternationalConferenceon
Audio-andVideo-BasedBiometricPersonAuthentication;Springer:Berlin/Heidelberg,Germany,2005.
21. Agarwal,A.;Singh,R.;Vatsa,M.Fingerprintsensorclassificationviamélangeofhandcraftedfeatures.InProceedingsofthe2016
23rdInternationalConferenceonPatternRecognition(ICPR),Cancun,Mexico,4–8December2016.
22. Zhao, Q.; Jain, A.; Abramovich, G. 3D to 2D fingerprints: Unrolling and distortion correction. In Proceedings of the 2011
InternationalJointConferenceonBiometrics(IJCB),Washington,DC,USA,11–13October2011.
23. Drahansky,M.;Dolezel,M.;Urbanek,J.;Brezinova,E.;Kim,T.H.Influenceofskindiseasesonfingerprintrecognition.J.Biomed.
Biotechnol.2012,2012,626148.[CrossRef]
24. Pillai,A.;Mil’shtein,S.Cancontactlessfingerprintsbecomparedtoexistingdatabase?InProceedingsofthe2012IEEEConference
onTechnologiesforHomelandSecurity(HST),Waltham,MA,USA,13–15November2012.
25. ISO/IEC2382-37;Biometrics,I.I.J.S.2017InformationTechnology-Vocabulary-Part37:Biometrics.InternationalOrganizationfor
Standardization:Geneva,Switzerland,2017.
26. Yin,X.;Zhu,Y.;Hu,J.ASurveyon2Dand3DContactlessFingerprintBiometrics:ATaxonomy,Review,andFutureDirections.
IEEEOpenJ.Comput.Soc.2021,2,370–381.[CrossRef]
27. Shafaei,S.;Inanc,T.;Hassebrook,L.G.Anewapproachtounwrapa3-Dfingerprinttoa2-Drolledequivalentfingerprint.In
Proceedingsofthe2009IEEE3rdInternationalConferenceonBiometrics:Theory,Applications,andSystems,Washington,DC,
USA,28–30September2009.
28. Affonso,C.;Rossi,A.L.D.;Vieira,F.H.A.;deLeonFerreira,A.C.P.Deeplearningforbiologicalimageclassification.ExpertSyst.
Appl.2017,85,114–122.[CrossRef]
29. Cai,L.;Gao,J.;Zhao,D.Areviewoftheapplicationofdeeplearninginmedicalimageclassificationandsegmentation. Ann.
Transl.Med.2020,8,713.[CrossRef][PubMed]
30. Wu,M.;Chen,L.Imagerecognitionbasedondeeplearning. InProceedingsofthe2015IEEEChineseAutomationCongress
(CAC),Wuhan,China,27–29November2015.
31. Pak,M.;Kim,S.Areviewofdeeplearninginimagerecognition. InProceedingsofthe20174thInternationalConferenceon
ComputerApplicationsandInformationProcessingTechnology(CAIPT),KutaBali,Indonesia,8–10August2017.
32. Wu,R.;Yan,S.;Shan,Y.;Dang,Q.;Sun,G.Deepimage:Scalingupimagerecognition.arXiv2015,arXiv:1501.02876.
33. Li, Y. Research and application of deep learning in image recognition. In Proceedings of the 2022 IEEE 2nd International
ConferenceonPower,ElectronicsandComputerApplications(ICPECA),Shenyang,China,21–23January2022.

J.Cybersecur.Priv.2022,2 729
34. Jia, X. Image recognition method based on deep learning. In Proceedings of the 2017 29th Chinese Control and Decision
Conference(CCDC),Chongqing,China,28–30May2017.
35. Cheng,F.;Zhang,H.;Fan,W.;Harris,B.Imagerecognitiontechnologybasedondeeplearning.Wirel.Per.Commun.2018,102,
1917–1933.[CrossRef]
36. Coates, A.; Ng, A.Y. Learning feature representations with k-means. In Neural Networks: Tricks of the Trade; Springer:
Berlin/Heidelberg,Germany,2012;pp.561–580.
37. Zhong,G.;Wang,L.N.;Ling,X.;Dong,J.Anoverviewondatarepresentationlearning:Fromtraditionalfeaturelearningtorecent
deeplearning.J.Financ.DataSci.2016,2,265–278.[CrossRef]
38. Minaee,S.;Abdolrashidi,A.;Su,H.;Bennamoun,M.;Zhang,D.Biometricsrecognitionusingdeeplearning:Asurvey.arXiv
2019,arXiv:1912.00271.
39. Kumar,A.Contactless3DFingerprintIdentification;Springer:Cham,Switzerland,2018.
40. Jia, W.; Yi, W.J.; Saniie, J.; Oruklu, E. 3D image reconstruction and human body tracking using stereo vision and Kinect
technology.InProceedingsofthe2012IEEEInternationalConferenceonElectro/InformationTechnology,Indianapolis,IN,USA,
6–8May2012.
41. Yin,X.;Zhu,Y.;Hu,J.3Dfingerprintrecognitionbasedonridge-valley-guided3Dreconstructionand3Dtopologypolymer
featureextraction.IEEETrans.PatternAnal.Mach.Intell.2019,43,1085–1091.[CrossRef]
42. Song,P.;Yu,H.;Winkler,S.Vision-based3Dfingerinteractionsformixedrealitygameswithphysicssimulation.InProceedings
ofthe7thACMSIGGRAPHInternationalConferenceonVirtual-RealityContinuumandItsApplicationsinIndustry,Singapore,
8–9December2008.
43. Liu,F.;Zhang,D.3Dfingerprintreconstructionsystemusingfeaturecorrespondencesandpriorestimatedfingermodel.Pattern
Recognit.2014,47,178–193.[CrossRef]
44. Liu,F.;Zhang,D.;Shen,L.Studyonnovelcurvaturefeaturesfor3Dfingerprintrecognition.Neurocomputing2015,168,599–608.
[CrossRef]
45. Nayar,S.K.;Gupta,M.Diffusestructuredlight. InProceedingsofthe2012IEEEInternationalConferenceonComputational
Photography(ICCP),Seattle,WA,USA,28–29April2012.
46. Zhang,L.;Curless,B.;Seitz,S.M.Rapidshapeacquisitionusingcolorstructuredlightandmulti-passdynamicprogramming.In
ProceedingsoftheFirstInternationalSymposiumon3DDataProcessingVisualizationandTransmission,Padova,Italy,19–21
June2002.
47. Kumar,A.;Kwong,C.Towardscontactless,low-costandaccurate3Dfingerprintidentification. InProceedingsoftheIEEE
ConferenceonComputerVisionandPatternRecognition,Portland,OR,USA,23–28June2013.
48. Parziale,G.Touchlessfingerprintingtechnology.InAdvancesinBiometrics;Springer:London,UK,2008;pp.25–48.
49. Carney, L.A.; Kane, J.; Mather, J.F.; Othman, A.; Simpson, A.G.; Tavanai, A.; Tyson, R.A.; Xue, Y. Amulti-finger touchless
fingerprintingsystem: Mobilefingerphotoandlegacydatabaseinteroperability. InProceedingsofthe20174thInternational
ConferenceonBiomedicalandBioinformaticsEngineering,Seoul,Korea,12–14November2017.
50. Rilvan,M.A.;Chao,J.;Hossain,M.S.Capacitiveswipegesturebasedsmartphoneuserauthenticationandidentification. In
Proceedingsofthe2020IEEEConferenceonCognitiveandComputationalAspectsofSituationManagement(CogSIMA),Victoria,
BC,Canada,24–29August2020.
51. Xie,W.;Song,Z.;Chung,R.C.Real-timethree-dimensionalfingerprintacquisitionviaanewphotometricstereomeans.Opt.Eng.
2013,52,103103.[CrossRef]
52. Zhang,D.;Lu,G.3Dbiometricstechnologiesandsystems.In3DBiometrics;Springer:NewYork,NY,USA,2013;pp.19–33.
53. Jecic´,S.;Drvar,N.Theassessmentofstructuredlightandlaserscanningmethodsin3Dshapemeasurements.InProceedingsof
the4thInternationalCongressofCroatianSocietyofMechanics,Bizovac,Croatia,18–20September2003.
54. Bell,T.;Li,B.;Zhang,S.Structuredlighttechniquesandapplications.InWileyEncyclopediaofElectricalandElectronicsEngineering;
JohnWiley&Sons,Inc.:Hoboken,NJ,USA,1999;pp.1–24.
55. Salih, Y.; Malik, A.S. Depth and geometry from a single 2D image using triangulation. In Proceedings of the 2012 IEEE
InternationalConferenceonMultimediaandExpoWorkshops,Melbourne,Australia,9–13July2012.
56. Labati,R.D.;Genovese,A.;Piuri,V.;Scotti,F.Towardunconstrainedfingerprintrecognition:Afullytouchless3-Dsystembased
ontwoviewsonthemove.IEEETrans.Syst.ManCybern.Syst.2015,46,202–219.[CrossRef]
57. Liu,F.;Zhang,D.;Song,C.;Lu,G.Touchlessmultiviewfingerprintacquisitionandmosaicking.IEEETrans.Instrum.Meas.2013,
62,2492–2502.[CrossRef]
58. Sero,D.;Garachon,I.;Hermens,E.;Liere,R.V.;Batenburg,K.J.Thestudyofthree-dimensionalfingerprintrecognitionincultural
heritage:Trendsandchallenges.J.Comput.Cult.Herit.2021,14,1–20.[CrossRef]
59. Genovese,A.;Munoz,E.;Piuri,V.;Scotti,F.;Sforza,G.Towardstouchlessporefingerprintbiometrics:Aneuralapproach.In
Proceedingsofthe2016IEEECongressonEvolutionaryComputation(CEC),Vancouver,BC,Canada,24–29July2016.
60. Piuri,V.;Scotti,F.Fingerprintbiometricsvialow-costsensorsandwebcams.InProceedingsofthe2008IEEESecondInternational
ConferenceonBiometrics:Theory,ApplicationsandSystems,Washington,DC,USA,29September–1October2008.
61. Deb,D.;Chugh,T.;Engelsma,J.;Cao,K.;Nain,N.;Kendall,J.;Jain,A.K.Matchingfingerphotostoslapfingerprintimages.arXiv
2018,arXiv:1804.08122.

J.Cybersecur.Priv.2022,2 730
62. Priesnitz,J.;Huesmann,R.;Rathgeb,C.;Buchmann,N.;Busch,C.Mobilecontactlessfingerprintrecognition:Implementation,
performanceandusabilityaspects.Sensors2022,22,792.[CrossRef][PubMed]
63. Wang,K.;Jiang,J.;Cao,Y.;Xing,X.;Zhang,R.Preprocessingalgorithmresearchoftouchlessfingerprintfeatureextractionand
matching.InChineseConferenceonPatternRecognition;Springer:Singapore,2016.
64. Liu,K.;Gong,D.;Meng,F.;Chen,H.;Wang,G.G.Gesturesegmentationbasedonatwo-phaseestimationofdistributionalgorithm.
Inf.Sci.2017,394,88–105.[CrossRef]
65. Bhattacharyya,D.;Ranjan,R.;Alisherov,F.;Choi,M.Biometricauthentication:Areview. Int. J.u-e-Serv. Sci. Technol. 2009,2,
13–28.
66. Khalil,M.S.;Wan,F.K.Areviewoffingerprintpre-processingusingamobilephone.InProceedingsofthe2012International
ConferenceonWaveletAnalysisandPatternRecognition,Xi’an,China,15–17July2012.
67. Khalil,M.S.;Kurniawan,F.;Saleem,K.Authenticationoffingerprintbiometricsacquiredusingacellphonecamera:Areview.Int.
J.WaveletsMultiresolut.Inf.Process.2013,11,1350033.[CrossRef]
68. Kaur,A.;Kranthi,B.ComparisonbetweenYCbCrcolorspaceandCIELabcolorspaceforskincolorsegmentation.Int.J.Appl.Inf.
Syst.2012,3,30–33.
69. Tassis,L.M.;deSouza,J.E.T.;Krohling,R.A.Adeeplearningapproachcombininginstanceandsemanticsegmentationtoidentify
diseasesandpestsofcoffeeleavesfromin-fieldimages.Comput.Electron.Agric.2021,186,106191.[CrossRef]
70. Priesnitz,J.;Rathgeb,C.;Buchmann,N.;Busch,C.Touchlessfingerprintsamplequality:Prerequisitesfortheapplicabilityof
NFIQ2.0.InProceedingsofthe2020InternationalConferenceoftheBiometricsSpecialInterestGroup(BIOSIG),Darmstadt,
Germany,16–18September2020.
71. Chinnappan,C.;Porkodi,R.FingerprintRecognitionTechnologyUsingDeepLearning: AReview. SSRNElectron. J.2021,9,
4647–4663.
72. Wu,Q.;Zhou,D.-X.Analysisofsupportvectormachineclassification.J.Comput.Anal.Appl.2006,8.
73. Zhang,Y.Supportvectormachineclassificationalgorithmanditsapplication.InInternationalConferenceonInformationComputing
andApplications;Springer:Berlin/Heidelberg,Germany,2012.
74. Gowri,D.S.;Amudha,T.Areviewonmammogramimageenhancementtechniquesforbreastcancerdetection.InProceedingsof
the2014InternationalConferenceonIntelligentComputingApplications,Coimbatore,India,6–7March2014.
75. FenshiaSingh,J.;Magudeeswaran,V.Amachinelearningapproachforbrainimageenhancementandsegmentation. Int. J.
ImagingSyst.Technol.2017,27,311–316.[CrossRef]
76. Gragnaniello,D.;Poggi,G.;Sansone,C.;Verdoliva,L.Localcontrastphasedescriptorforfingerprintlivenessdetection.Pattern
Recognit.2015,48,1050–1058.[CrossRef]
77. Hu,Z.;Li,D.;Isshiki,T.;Kunieda,H.HybridMinutiaeDescriptorforNarrowFingerprintVerification. IEICETrans. Inf. Syst.
2017,100,546–555.[CrossRef]
78. Svoboda,J.DeepLearningfor3DHandBiometricSystems;UniversitàdellaSvizzeraItaliana:Lugano,Switzerland,2020.
79. Zhang,Z.;Liu,S.;Liu,M.Amulti-taskfullydeepconvolutionalneuralnetworkforcontactlessfingerprintminutiaeextraction.
PatternRecognit.2021,120,108189.[CrossRef]
80. Zhou,W.;Hu,J.;Petersen,I.;Wang,S.;Bennamoun,M.Abenchmark3Dfingerprintdatabase.InProceedingsofthe201411th
InternationalConferenceonFuzzySystemsandKnowledgeDiscovery(FSKD),Xiamen,China,19–21August2014.
81. Melekhov,I.;Ylioinas,J.;Kannala,J.;Rahtu,E.Image-basedlocalizationusinghourglassnetworks.InProceedingsoftheIEEE
InternationalConferenceonComputerVisionWorkshops,Venice,Italy,22–29October2017.
82. Jiang,L.;Zhao,T.;Bai,C.;Yong,A.;Wu,M.Adirectfingerprintminutiaeextractionapproachbasedonconvolutionalneural
networks.InProceedingsofthe2016InternationalJointConferenceonNeuralNetworks(IJCNN),Vancouver,BC,Canada,24–29
July2016.
83. Lin,C.;Kumar,A.ACNN-basedframeworkforcomparisonofcontactlesstocontact-basedfingerprints.IEEETrans.Inf.Forensics
Secur.2018,14,662–676.[CrossRef]
84. Yu,W.;Yang,K.;Bai,Y.;Xiao,T.;Yao,H.;Rui,Y.VisualizingandcomparingAlexNetandVGGusingdeconvolutionallayers.In
Proceedingsofthe33rdInternationalConferenceonMachineLearning,NewYorkCity,NY,USA,19–24June2016.
85. Ballester,P.;Araujo,R.M.OntheperformanceofGoogLeNetandAlexNetappliedtosketches.InProceedingsoftheThirtieth
AAAIConferenceonArtificialIntelligence,Pelotas,Brazil,21February2016.
86. Lin,C.;Kumar,A.Contactlessandpartial3Dfingerprintrecognitionusingmulti-viewdeeprepresentation. PatternRecognit.
2018,83,314–327.[CrossRef]
87. Lin, C.; Kumar, A. Improving cross sensor interoperability for fingerprint identification. In Proceedings of the 2016 23rd
InternationalConferenceonPatternRecognition(ICPR),Cancun,Mexico,4–8December2016.
88. Watson,C.I.;Garris,M.D.;Tabassi,E.;Wilson,C.L.;McCabe,R.M.;Janet,S.;Ko,K.User’sGuidetoNISTBiometricImageSoftware
(NBIS);NIST:Gaithersburg,MD,USA,2007.
89. Tan,H.;Kumar,A.Minutiaeattentionnetworkwithreciprocaldistancelossforcontactlesstocontact-basedfingerprintidentifica-
tion.IEEETrans.Inf.ForensicsSecur.2021,16,3299–3311.[CrossRef]
90. Sundararajan,K.;Woodard,D.L.Deeplearningforbiometrics:Asurvey.ACMComput.Surv.CSUR2018,51,1–34.[CrossRef]