|     |     |     |     |     |     |     |     | TYPE      | OriginalResearch |     |     |
| --- | --- | --- | --- | --- | --- | --- | --- | --------- | ---------------- | --- | --- |
|     |     |     |     |     |     |     |     | PUBLISHED | 05December2025   |     |     |
10.3389/fdata.2025.1641714
DOI
|     | Robust         |     | contactless |     |     |       | fingerprint |         |     |     |     |
| --- | -------------- | --- | ----------- | --- | --- | ----- | ----------- | ------- | --- | --- | --- |
|     | authentication |     |             |     |     | using |             | dolphin |     |     |     |
OPENACCESS
| EDITEDBY | optimization |     |     |     | and |     | SVM |     |     |     |     |
| -------- | ------------ | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
ChenWang,
HuazhongUniversityofScienceand
| Technology,China | hybridization |     |     |     |     |     |     |     |     |     |     |
| ---------------- | ------------- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
REVIEWEDBY
N.R.Pradeep,
NavkisCollegeofengineering,India
| HajarMaseeh, | JenishaRachelandEzhilmaranDevarasan* |     |     |     |     |     |     |     |     |     |     |
| ------------ | ------------------------------------ | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
AkreUniversityforAppliedSciences,Iraq
DepartmentofMathematics,SchoolofAdvancedSciences,VelloreInstituteofTechnology,Vellore,
*CORRESPONDENCE
Tamilnadu,India
EzhilmaranDevarasan
ezhil.devarasan@yahoo.com
RECEIVED05June2025
Thefieldofcontactlessfingerprint(CLFP)recognitionisrapidlyevolving,driven
REVISED05November2025
byitspotentialtoofferenhancedhygieneanduserconvenienceovertraditional
ACCEPTED13November2025
PUBLISHED05December2025 touch-based systems without compromising security. This study introduces
|     | a contactless |     | fingerprint | recognition |     | system | using | the | Dolphin | Optimization |     |
| --- | ------------- | --- | ----------- | ----------- | --- | ------ | ----- | --- | ------- | ------------ | --- |
CITATION
RachelJandDevarasanE(2025)Robust Algorithm (DOA), a nature-inspired technique suited for complex optimization
contactlessfingerprintauthenticationusing
tasks.TheHistogramofOrientedGradients(HOG)methodisappliedtoreduce
dolphinoptimizationandSVMhybridization.
|     | image features, |     | with | DOA | optimizing | the | feature | selection | process. |     | To boost |
| --- | --------------- | --- | ---- | --- | ---------- | --- | ------- | --------- | -------- | --- | -------- |
Front.BigData8:1641714.
doi:10.3389/fdata.2025.1641714
|     | prediction | accuracy, |     | we fused | the | DOA with | a   | Support | Vector | Machine | (SVM) |
| --- | ---------- | --------- | --- | -------- | --- | -------- | --- | ------- | ------ | ------- | ----- |
COPYRIGHT classifier,creatingahybrid(DOA-SVM)thatleveragestheglobalsearchprowess
©2025 RachelandDevarasan.Thisisan of DOA alongside the reliable classification strength of SVM. Additionally, two
open-accessarticledistributedunderthe
termsoftheCreativeCommonsAttribution more hybrid models are proposed: one combining Fuzzy C-Means (FCM) with
License(CCBY).Theuse,distributionor DOA-SVM, and another combining Neutrosophic C-Means (NCM) with DOA-
reproductioninotherforumsispermitted,
|     | SVM. Experimental |     | validation |     | on  | 504 contactless |     | fingerprint | images |     | from the |
| --- | ----------------- | --- | ---------- | --- | --- | --------------- | --- | ----------- | ------ | --- | -------- |
providedtheoriginalauthor(s)andthe
|     | Hong Kong | Polytechnic |     | University |     | dataset | demonstrates |     | a clear | performance |     |
| --- | --------- | ----------- | --- | ---------- | --- | ------- | ------------ | --- | ------- | ----------- | --- |
copyrightowner(s)arecreditedandthatthe
originalpublicationinthisjournaliscited,in progression: DOA (91.00%), DOA-SVM (94.07%), FCM-DOA-SVM (96.03%), and
accordancewithacceptedacademicpractice.
|     | NCM-DOA-SVM |     | (98.00%). |     | The NCM-DOA-SVM |     |     | approach | achieves |     | superior |
| --- | ----------- | --- | --------- | --- | --------------- | --- | --- | -------- | -------- | --- | -------- |
Nouse,distributionorreproductionis
|     | accuracy | through | effective |     | uncertainty | handling |     | via neutrosophic |     | logic | while |
| --- | -------- | ------- | --------- | --- | ----------- | -------- | --- | ---------------- | --- | ----- | ----- |
permittedwhichdoesnotcomplywiththese
maintainingcompetitiveprocessingefficiency.Comparativeanalysiswithother
terms.
|     | bio-inspired | methods       |     | shows         | our | approach | achieves |           | higher | accuracy      | with |
| --- | ------------ | ------------- | --- | ------------- | --- | -------- | -------- | --------- | ------ | ------------- | ---- |
|     | reduced      | computational |     | requirements. |     | These    | results  | highlight | the    | effectiveness |      |
ofcombiningbio-inspiredoptimizationwithtraditionalclassifiersandadvanced
clusteringforbiometricrecognition.
KEYWORDS
contactlessfingerprint,featureextraction,HOGalgorithm,machinelearning,dolphin
optimizationalgorithm,clustering
1 Introduction
Biometricauthenticationsystemshavegainedvitalimportanceinsecurityapplications
|     | due to their | ability | to  | verify individuals |     | based | on distinct | physical | or  | behavioral | traits. |
| --- | ------------ | ------- | --- | ------------------ | --- | ----- | ----------- | -------- | --- | ---------- | ------- |
Amongthevariousbiometricmodalities,fingerprint(FP)recognitionremainsoneofthe
mostextensivelyusedmethodsduetoitsuniqueness,permanence,anduseracceptance.
Conventionalsystems,however,requirephysicalcontactwithasensor,raisingconcerns
abouthygiene,latentFPsecurity,andpotentialuserresistance(Maltonietal.,2009;Vibert
etal.,2023).
TheidentificationofCLFPhasalsobeenproposedasapossiblesubstitute,enabling
|                    | the recognition |            | of FP | without     | physical | touch         | with a | sensor  | (Grosz et    | al., 2021).     | This       |
| ------------------ | --------------- | ---------- | ----- | ----------- | -------- | ------------- | ------ | ------- | ------------ | --------------- | ---------- |
|                    | method          | eliminates | the   | limitations | of       | contact-based |        | systems | while aiming |                 | to deliver |
| FrontiersinBigData |                 |            | 01    |             |          |               |        |         |              | frontiersin.org |            |

RachelandDevarasan 10.3389/fdata.2025.1641714
comparableauthenticationperformance.Developmentandlarge- Overall, HOG has advantages over traditional features like Local
scale applications of CLFP recognition technologies have been Binary Patterns (LBP) or Scale-Invariant Feature Transform
motivated by the widespread availability of high-quality digital (SIFT) for capturing important texture and edge information
cameras in smartphones and other handheld devices. Despite all for fingerprint recognition while also improving computational
these developments, CLFP still face serious challenges that affect efficiency compared to deep learning methods. SVM is chosen
performanceanddependability. as the base classifier due to its superior performance in high-
EvenwiththebenefitsthatCLFPsystemsconfer,therearemany dimensional feature spaces and strong theoretical foundation.
challengesfortheactualpracticaluseofsuchsystems,whichcan SVM’sabilitytohandlenon-linearlyseparabledatathroughkernel
be long-lasting. The performance can be severely compromised functionsmakesitidealforcomplexbiometricclassificationtasks
by degradation of image quality, attributable to factors like comparedtosimplerclassifierslikeK-NearestNeighbors(KNN)or
variations in illumination levels, capture distance to the finger, DecisionTrees.
angular distortions and focus irregularity. Meanwhile, artifacts Despite considerable progress in CLFP recognition, three
introduced by capturing the finger image contactless give rise to significantresearchgapsremainunaddressed.First,thereislimited
non-linear geometric distortions due to varying finger pressure exploration of bio-inspired optimization algorithms like DOA
and orientation, which complicates feature extraction compared for simultaneous feature selection and parameter optimization
tocontact-basedsystems.Traditionalmachinelearningclassifiers in CLFP systems. Second, current approaches rarely address
oftenstrugglewiththeinherentvariabilityinCLFPimages,while the inherent indeterminacy and noise in CLFP images using
conventionalparameteroptimizationmethodsarecomputationally advancedmathematicalframeworkslikeneutrosophiclogic.Third,
expensiveandfrequentlyyieldsub-optimalresults.Table1provides most studies focus on large datasets with insufficient analysis
thesummaryofrecentCLFPresearchstudies. of computational efficiency, which is crucial for real-world
Most works focus on either deep learning or optimization deployment.
techniques,withfewstudiesintegratingbio-inspiredoptimization Toaddresstheidentifiedresearchgaps,thispaperintroduces
algorithmswithadvancedclusteringmethodsforCLFPrecognition andrigorouslyevaluatesaseriesofnovelhybridmodelsforCLFP
(Yin et al., 2021; Chowdhury and Imtiaz, 2022; Sreehari and recognition.Ourkeycontributionsare:
Anzar, 2025). Current research lacks systematic approaches to
address indeterminacy in CLFP images using neutrosophic or • WeproposeastandaloneDOA-basedclassifier,establishinga
similarfuzzyclusteringmethodologies.Limitedattentionhasbeen baselineusingthisunexploredoptimizerforCLFP.
given to optimizing feature selection through nature-inspired • WedevelopaDOA-SVMhybrid,effectivelycombiningDOA’s
algorithms such as the DOA, particularly when combined with globalsearchcapabilitieswithSVM’sdiscriminativepowerfor
SVM or clustering techniques (Chowdhury and Imtiaz, 2022; enhancedclassification.
Sreehari and Anzar, 2025). While many studies utilize large • WeenhancethishybridfurtherbyintegratingFuzzyC-Means
publicdatasets;fewreportresultsonsmaller,challengingdatasets (FCM) clustering, creating the FCM-DOA-SVM model to
(e.g., 504 images) with detailed computational efficiency analysis improvefeaturerepresentationthroughsoftclustering.
(Attrish et al., 2021). Despite significant advances in the field, • As our core innovation, we introduce the NCM-DOA-SVM
persistentchallengesremaininhandlingposevariation,achieving hybrid.ThismodelleveragesNeutrosophicC-Means(NCM)
sensor interoperability, and developing robust liveness detection clustering to directly quantify and manage the uncertainty
mechanismsforCLFPsystems. inherent in CLFP images, thereby increasing robustness in
This research strategically selects the DOA as the core challengingconditions.
optimization technique based on several compelling advantages
over contemporary bio-inspired algorithms. Unlike Particle We empirically validated all four approaches on a dataset
Swarm Optimization (PSO) which may suffer from premature of 504 CLFP images from the PolyU database. Our evaluation
convergence,orGeneticAlgorithms(GA)withhighcomputational providesacomprehensivebenchmark,reportingonbothaccuracy
complexity,DOAdemonstratessuperiorglobalsearchcapabilities andcomputationalefficiencytooffervaluableinsightsforpractical,
through its unique echolocation-inspired mechanism. DOA’s resource-consciousapplications.
adaptive search behavior, inspired by dolphin hunting strategies, The outline of the paper is as follows: Section 2 presents
provides better exploration-exploitation balance compared to a literature on related work related to CLFP recognition,
WhaleOptimizationAlgorithm(WOA),AntColonyOptimization optimization algorithms, and clustering methods. Section 3
(ACO), or Gray Wolf Optimizer (GWO), particularly crucial describes the proposed methodology. Section 4 presents the
for complex parameter optimization in biometric systems. The experimental setup, the findings and the thorough discussion.
selection of Histogram of Oriented Gradients (HOG) for feature Finally,Section5concludesthepaperandsuggestsfuturework.
extraction is motivated by its robustness to illumination changes
and geometric variations—critical factors in CLFP systems.
2 Related work
Abbreviations:G,gradientmagnitude;θ,gradientangle;A1,A2,acceleration CLFP recognition has gained significant attention in recent
coefficients;ω,inertiaweight;ν 1,ν 2,randomnumbersuniformlydistributed yearsasanalternativetotraditionalcontact-basedsystems.(Jawade
in[0,1];s,currenttimestep;ψ,timesteps;(cid:6),totalnumberofdolphins;A, et al., 2022) proposed one of the early CLFP identification
penaltyparameter;(cid:7),lossfunction;ϑ,kernelparameter. systems using level zero features, demonstrating the feasibility
FrontiersinBigData 02 frontiersin.org

| RachelandDevarasan |     |     |     | 10.3389/fdata.2025.1641714 |     |
| ------------------ | --- | --- | --- | -------------------------- | --- |
TABLE1 SummaryofrecentCLFPstudies:task,approach,novelty,dataset.
| References      | Maintask |     | Novelty/focus         | Datasetused |     |
| --------------- | -------- | --- | --------------------- | ----------- | --- |
| (Yinetal.,2020) | Matching |     | Perspectivedistortion | 2datasets   |     |
(TanandKumar,2020) Minutiae/Matching Pose-invariantmatching Public,custom
(Zhangetal.,2021) MinutiaeExtraction Jointlocation/direction 3datasets
| (Groszetal.,2021)   | Matching |     | Mobile,cross-database | 3datasets |     |
| ------------------- | -------- | --- | --------------------- | --------- | --- |
| (Attrishetal.,2021) | Matching |     | Real-time,JetsonNano  | IITI-CFD  |     |
(Shietal.,2022) Matching Posture/rotationrobustness PolyU,FVC2004
| (DongandKumar,2023) | Matching |     | Cross-modality,privacy | Custom |     |
| ------------------- | -------- | --- | ---------------------- | ------ | --- |
(Mohamed-Abdul-Caderetal.,2023) Matching Multi-finger,ridgeorientation PolyU,others
(Kapleshetal.,2024) Classification ViTforCLFP ISPFDv1/v2,UNFIT
| (Cuietal.,2024) | Matching |     | 3Dminutiae,pose | Custom |     |
| --------------- | -------- | --- | --------------- | ------ | --- |
(Ruzickaetal.,2025) Segmentation Fingertipsegmentation,FPN Custom
of this approach. (Labati et al., 2014) provided a comprehensive i.e., the regularization parameter C and kernel parameters (such
survey of 2D and 3D touchless FP technologies, highlighting as gamma in the case of the RBF kernel). Traditional parameter
the challenges and opportunities in this domain. More recently, optimization methods like grid search and random search are
investigated the performance and standards for CLFP capture, generallycomputationallydemandingandmaynotalwaysgivethe
| emphasizingtheneedforrobustalgorithmstoaddresstheinherent |     |     | bestresult. |     |     |
| --------------------------------------------------------- | --- | --- | ----------- | --- | --- |
variability in contact-less acquisition. (Priesnitz et al., 2021) The FCM approach, introduced by (Bezdek et al., 1984), is
conducted an in-depth review of touchless 2D FP recognition, an extension of traditional clustering methods in the aspect that
surveying recent approaches and their limitations. Despite such it allows data points to be a member of multiple clusters to
advancements, CLFP recognition remains hampered by image varyingdegrees.Thisapproachisparticularlyusefulforbiometric
quality, feature extraction, and classification performance-related data when the boundaries of features are fuzzy. The FCM
issues. Traditional approaches are generally less robust against approach has been applied to numerous biometric recognition
variability in CLFP images and hence necessitate the use systems, including FP, face, and iris recognition. Its ability
of more robust optimization and classification methods. Bio- to accommodate uncertainty in feature representation makes it
inspired optimization algorithms have demonstrated exceptional an attractive tool for enhancing the effectiveness of biometric
effectivenessinthesolutionofcomplexoptimizationproblemsin classificationsystems.
variousdisciplines.TheDOA,proposedby(KavehandFarhoudi, NCM is based on neutrosophic set theory introduced by
2013), simulates the dolphin’s echolocation capability to detect (Smarandache,2003)andimplementedasaclusteringalgorithmby
optimal solutions within a given range of search. The algorithm (GuoandSengur,2015),extendsfuzzyclusteringbyincorporating
wasutilizedeffectivelyinvariousoptimizationproblems,including the dimension of indeterminacy. In NCM, each data point is
feature selection and parameter optimization in classification described by three membership functions: truth (belonging to a
problems.Otherbio-inspiredapproacheshavealsobeenexplored cluster), indeterminacy (uncertainty), and falsity (non-belonging
for parameter optimization in machine learning (ML) systems. to a cluster). This approach is particularly beneficial for CLFP
(Huang and Wang, 2006) proposed a genetic algorithm-based images, where quality can be extremely inconsistent and certain
approach for feature selection and parameter optimization in regionscancontainindeterminateinformationviablur,shadow,or
| SVM and demonstrated | improved classification | performance. | otherartifacts. |     |     |
| -------------------- | ----------------------- | ------------ | --------------- | --- | --- |
Similarly, (Lin et al., 2008) used particle swarm optimization Hybrid approaches combining optimization algorithms, ML
for optimizing SVM parametersand feature selection. Theabove classifiers, and clustering techniques have shown promising
studiesreflectthepopularityofutilizingbio-inspiredoptimization results in various biometric recognition tasks. These approaches
algorithms for the performance improvement of ML classifiers. leverage the strengths of individual methods to address the
Their application in CLFP recognition, however, particularly challenges in biometric recognition, particularly in contact-less
in the presence of advanced clustering techniques, is relatively scenarios. However, despite the potential of hybrid approaches,
new. thereislimitedresearchoncombiningbio-inspiredoptimization
SVM first proposed by (Cortes and Vapnik, 1995), has been algorithmslikeDOAwithadvancedclusteringtechniqueslikeFCM
widelyusedinbiometricrecognitionsystemsduetotheirrigorous andNCMforCLFPrecognition.Thisresearchgapmotivatesour
theoreticalfoundationandbetterperformanceinhigh-dimensional proposed frame-work, which aims to improve the accuracy and
spaces. SVM finds the best hyperplane with the largest class robustnessofCLFPrecognitionthroughnovelhybridapproaches.
separabilityandhenceiswellsuitedtobiometricclassifyingtasks. Table2providesarecentstudyonthemethodsanalyzedonPolyU
| SVMperformanceishighlydependentonparameteroptimization, |     |     | CLFPDataset. |     |                 |
| ------------------------------------------------------- | --- | --- | ------------ | --- | --------------- |
| FrontiersinBigData                                      |     |     | 03           |     | frontiersin.org |

| RachelandDevarasan |     |     |     |     |     |     |     |     |     |     | 10.3389/fdata.2025.1641714 |     |     |
| ------------------ | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | -------------------------- | --- | --- |
TABLE2 SummaryofrecentCLFPstudies:task,approach,novelty,dataset.
| References |     | Maintask |     |     |     | Keytechniques |     |     |     |     | Method |     |     |
| ---------- | --- | -------- | --- | --- | --- | ------------- | --- | --- | --- | --- | ------ | --- | --- |
(Peddietal.,2025) Minutiaelocalization,matching Groupedmulti-scalegraph-involution,end-to-endlearning G-MSGINet
(Artan,2024) Matching MinNetarchitecture,multi-scalefeatures MinNet
(Siddiquietal.,2024) Enhancement,minutiaeextraction,matching Frequencyestimation,minutiaeencoding Offline+onlinephase
(Rajarametal.,2023) Enhancement,featureextraction,matching CNN(Child-CLEF),hybridenhancement CNN
(Shietal.,2022) Matching Triplet-GAN,dataaugmentation TripletGAN
3 Methodology
andcv2.cvtColor()withthecv2.COLOR_RGB2GRAYflag
forcolor-to-grayscaleconversion.
| 3.1 Algorithms |     | overview |     |     |     |     |     |     |     |     |     |     |     |
| -------------- | --- | -------- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
Wehavedevelopedandevaluatedfourdistinctmethodologies 3.2.1 CLFPdatabase
| for CLFP | recognition. | The | overall architecture | of our | proposed |     |             |         |     |              |     |          |           |
| -------- | ------------ | --- | -------------------- | ------ | -------- | --- | ----------- | ------- | --- | ------------ | --- | -------- | --------- |
|          |              |     |                      |        |          | The | experiments | related | to  | CLFP utilize | a   | publicly | available |
framework is depicted in Figure1, which outlines the integrated dataset obtained from the Hong Kong Polytechnic University
pipeline for feature extraction, optimization, clustering, and (PolyU) (Lin and Kumar, 2018). This dataset consists of 2,016
classification. The proposed framework adopts a single-stage CLFP images representing 336 unique classes. All images are in
classification system which integrates feature extraction (HOG), BMP format with a resolution of 128 × 128 pixels. This dataset
| optimization | (DOA), | cluster | methodology | (FCM/NCM), |     | and          |       |       |     |             |     |                   |     |
| ------------ | ------ | ------- | ----------- | ---------- | --- | ------------ | ----- | ----- | --- | ----------- | --- | ----------------- | --- |
|              |        |         |             |            |     | was acquired | using | Canon | EOS | 450D camera |     | with standardized |     |
classifier(SVM)intoasingledecision-makingprocess.Thissingle- protocols (four LED lights at 45Âˇr, uniform background, 12cm
stage approach is chosen for this study to establish baseline distance) and processed through resizing (4,272 × 2,848 to
×
performanceandenabledirectcomparisonbetweenoptimization 128 128 pixels), grayscale conversion, histogram equalization,
algorithms, with the unified pipeline allowing for end-to-end Gaussianblur(σ=0.5),contrastenhancement(α=1.2,β=10),and
optimizationofallcomponentssimultaneously,ensuringcoherent
|     |     |     |     |     |     | normalization | with | quality | assessment |     | via Laplacian |     | variance, |
| --- | --- | --- | --- | --- | --- | ------------- | ---- | ------- | ---------- | --- | ------------- | --- | --------- |
parameter tuning across the entire system. The first approach is contrastmeasurement,andbrightnessanalysis.Thedatasetexhibits
a Standalone DOA that uses the DOA for both feature selection significant demographic limitations with participants primarily
and classifier parameter optimization. The next algorithm would comprisingEastAsianuniversitystudents/staff(92.3%EastAsian,
be the DOA-SVM Hybrid in which the DOA would be used ages 18–35, 60.1% male) under controlled laboratory conditions,
| for parameter | tuning | and | SVM is in | the classification | phase. |     |     |     |     |     |     |     |     |
| ------------- | ------ | --- | --------- | ------------------ | ------ | --- | --- | --- | --- | --- | --- | --- | --- |
substantiallylimitinggeneralizabilitytodiverseglobalpopulations,
The next framework improves on that hybrid by adding in agegroups,skinconditions,andreal-worlddeploymentscenarios.
FCM cluster methodology to get better feature representation, Forexperimentalpurposes,thedatasetisdividedintotwosubsets:
we will refer to this as the FCM-DOA-SVM Hybrid. Next, the 1,512imagesareusedfortrainingthesystem,andtheremaining
NCM-DOA-SVM Hybrid is the final framework which relies 504 images are used for testing. This setup aims to evaluate the
| on NCM | clustering | and | in an important | way utilizes |     | this     |            |             |     |             |       |          |       |
| ------ | ---------- | --- | --------------- | ------------ | --- | -------- | ---------- | ----------- | --- | ----------- | ----- | -------- | ----- |
|        |            |     |                 |              |     | system’s | ability to | distinguish |     | individuals | based | on their | CLFP. |
clusteringmethodologytodealwithuncertaintyandindeterminacy Figure2 illustrates a sample from the CLFP image database. The
commonly found in CLFP images. In all four approaches experiments are conducted on a system with an Intel Core Ultra
mentioned: data is pre-processed, to reduce dimensionality, and 7-155H processor (3.80 GHz), 32 GB RAM, and Python 3.12.3
featureisextractedfromtheinputimageswhichutilizestheHOG (conda-forge,[MSCv.193864-bit,AMD64]).
algorithm.
|     |     |     |     |     |     | 3.2.2 Ethicalconsiderationsandprivacy |     |     |     |     |     |     |     |
| --- | --- | --- | --- | --- | --- | ------------------------------------- | --- | --- | --- | --- | --- | --- | --- |
protection
| 3.2 Preprocessing |     |     | of input | image |     |               |      |          |            |          |               |              |        |
| ----------------- | --- | --- | -------- | ----- | --- | ------------- | ---- | -------- | ---------- | -------- | ------------- | ------------ | ------ |
|                   |     |     |          |       |     | All biometric |      | data     | collection | followed | institutional |              | ethics |
|                   |     |     |          |       |     | protocols     | with | informed | consent    | obtained | from          | participants |        |
Preprocessing is a critical first step that directly impacts regarding data usage for research purposes. The dataset is
recognitionaccuracybystandardizingimagequalityandpreparing anonymized with no personally identifiable information linked
the data for subsequent analysis. Our preprocessing pipeline to biometric samples. Participants were informed about data
consists of two key steps. Step 1 involves resizing the image to a storage duration, usage limitations, and their right to withdraw
fixed dimension of 128 × 128 pixels to ensure uniformity across consent. The study complies with biometric data protection
all input samples. Step 2 entails converting the image from RGB regulations and follows privacy-by-design principles with secure
(color)formattograyscale.Grayscaleimagesarecomputationally data handling protocols. However, the inherent permanence
less intensive and easier to process compared to color images, of biometric identifiers raises ongoing privacy considerations
rendering them more suitable for image analysis tasks. These regardingpotentialmisuse,cross-systemidentification,andlong-
preprocessing steps are implemented using OpenCV in Python, termdatasecuritythatusersshouldbeawareofwhenconsenting
specificallyemployingthecv2.resize()functionforresizing tobiometricresearchparticipation.
| FrontiersinBigData |     |     |     |     |     | 04  |     |     |     |     |     | frontiersin.org |     |
| ------------------ | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --------------- | --- |

| RachelandDevarasan |     |     |     |     |     |     | 10.3389/fdata.2025.1641714 |     |
| ------------------ | --- | --- | --- | --- | --- | --- | -------------------------- | --- |
FIGURE1
Architecturerepresentationoftheproposedframework.
| 3.3 Feature | extraction |     |     |     |              |                     |         |                |
| ----------- | ---------- | --- | --- | --- | ------------ | ------------------- | ------- | -------------- |
|             |            |     |     | 1.  | The gradient | value is determined | through | the subsequent |
steps:
| A significant | issue | encountered by | excellence algorithms |     |     |     |     |     |
| ------------- | ----- | -------------- | --------------------- | --- | --- | --- | --- | --- |
•
is the presence of redundant and superfluous attributes, Compute the gradient magnitude per pixel using the
which lead to an increase in data volume and consequently specifiedequation:
(cid:2)
| expand | the research space | needed to | address the problem. |     |     |                           |     |     |
| ------ | ------------------ | --------- | -------------------- | --- | --- | ------------------------- | --- | --- |
|        |                    |           |                      |     |     | G(u,v)= Gu(u,v)2+Gv(u,v)2 |     |     |
This expansion results in prolonged processing time for data (1)
| necessary | for detection | and classification | (Soranamageswari |     |     |     |     |     |
| --------- | ------------- | ------------------ | ---------------- | --- | --- | --- | --- | --- |
•
|            |                |                 |           |      | Determine | the gradient angle | for each | pixel through the |
| ---------- | -------------- | --------------- | --------- | ---- | --------- | ------------------ | -------- | ----------------- |
| and Meena, | 2010; Vithlani | and Kumbharana, | 2015). In | this |           |                    |          |                   |
correspondingequation:
| context, | we will utilize | the HOG algorithm | for extracting |     |     |     |     |     |
| -------- | --------------- | ----------------- | -------------- | --- | --- | --- | --- | --- |
(cid:3) (cid:4)
features, as this step is a critical phase in image processing. Gv(u,v)
θ(u,v)=arctan
The HOG algorithm will be employed to extract significant (2)
Gu(u,v)
| features | from images, | thereby facilitating | the classification |     |     |     |     |     |
| -------- | ------------ | -------------------- | ------------------ | --- | --- | --- | --- | --- |
process and reducing data size, which in turn decreases the 2. The image is partitioned into blocks of dimensions (2 × 2),
| time required | for classification | (Moen, | 2018; Cetina et | al., |     |     |     |     |
| ------------- | ------------------ | ------ | --------------- | ---- | --- | --- | --- | --- |
resultinginatotaloffourblocks.
| 2014). |     |     |     | 3.  | Eachblockundergoesanine-wayHOGextraction. |     |     |     |
| ------ | --- | --- | --- | --- | ----------------------------------------- | --- | --- | --- |
ThefundamentalstagesoftheHOGalgorithm(Elharirietal., 4. TheHOGblocksarecompiledintoaone-dimensionalfeature
2015)areoutlinedasfollows:
vector.
| FrontiersinBigData |     |     |     | 05  |     |     |     | frontiersin.org |
| ------------------ | --- | --- | --- | --- | --- | --- | --- | --------------- |

RachelandDevarasan 10.3389/fdata.2025.1641714
FIGURE2
SampleimagesfromtheCLFPdatabase.
FIGURE3
ExtractingfeaturesusingtheHOGalgorithm.
Figure3presentsaschematicrepresentationoftheutilization determined by selecting the appropriate parameters for the
oftheHOGalgorithmforextractingfeaturesfromCLFPimages. HOG, specifically focusing on the cell-size and block-size.
In this research, traits were extracted by identifying the The implementation of the HOG technique was executed in
most critical features that encapsulate the maximum amount Python utilizing the skimage.feature library, with the hog
of information from the dataset. These key features were function.
FrontiersinBigData 06 frontiersin.org

| RachelandDevarasan |     |     |     |     |     |     |     |     |     |     |     | 10.3389/fdata.2025.1641714 |     |     |
| ------------------ | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | -------------------------- | --- | --- |
3.4 Overfitting prevention strategy elements of the algorithm are a location update method, an
equationforspeedchangesandusingecholocationtofindthebest
Toensurerobustmodelperformanceandpreventoverfitting, solutions(Wuetal.,2017;Kavehetal.,2017).Thesemechanisms
we implemented a comprehensive validation framework within enable the algorithm to converge toward global optima while
our existing train-test split. The training set (1,512 images) was avoiding local traps, rendering DOA a robust and adaptable tool
furtherdividedusingfive-foldstratifiedcross-validation,ensuring fortacklingawidearrayofoptimizationchallenges.
balancedclassdistributionacrossallfolds.
3.5.1 MainequationsofDOA
3.4.1 Regularizedfitnessfunction The DOA simulates the social and hunting behavior of
To discourage overly complex models that might overfit, we dolphins.Theessentialmathematicalformulationsinvolvedinthe
modifiedtheDOA’sfitnessfunctiontoincorporateregularization algorithmisdescribedasfollows.
termsthatpenalizecomplexityandsuspiciouslyhighperformance Thefirststepinvolvesinitialpositioning,whereapopulationof
(ChiccoandJurman,2020): dolphinsisrandomlyinitializedwithinthesearchspace,imitating
dolphinsspreadingoutinsearchofprey.
|     |            |     |     |     |     |     |     | In  | the chasing | phase, | dolphins | update | their | velocities |
| --- | ---------- | --- | --- | --- | --- | --- | --- | --- | ----------- | ------ | -------- | ------ | ----- | ---------- |
|     | Fitnessreg |     | =   |     |     |     |     |     |             |        |          |        |       |            |
(cid:5) (ref Equation4) and positions (ref Equation5) based on the
|     |     |     | −λ  | n |pi | |−λ ·N | −λ  | ·   |     |     |     |     |     |     |     |
| --- | --- | --- | --- | ----- | ------ | --- | --- | --- | --- | --- | --- | --- | --- | --- |
Accuracy 1 i=1 2 features 3 (3) mathematicallydefinedtwokeyequations.
CV
I(Accuracy>0.98)
|       | λ   | =     | λ = |        | λ =  |     |                |     | m +1 | (s+1) |     |     |     |     |
| ----- | --- | ----- | --- | ------ | ---- | --- | -------------- | --- | ---- | ----- | --- | --- | --- | --- |
| where | 1   | 0.01, | 2   | 0.005, | 3 10 | are | regularization |     | Vel  |       |     |     |     |     |
i,j
co e ffi c i e n t s, p r ep r e se n t sm o de l p a ra m et e rs , N is th e n u m b e r = ω · m + · ν · ℘ m )−χ m
|     |     | i   |     |     |     | f e at ur e s |     |     | V   | el i,j ( s ) | A 1 1 ( | (         | i ,j (s)) | (4) |
| --- | --- | --- | --- | --- | --- | ------------- | --- | --- | --- | ------------ | ------- | --------- | --------- | --- |
|     |     |     | I(  | ·)  |     |               |     |     |     |              |         | b est,i,j |           |     |
o f s el e c t e d f e a tu re s , a n d i s a n i nd ic a t o r f u n c t io n p en a l iz in g + A · ν · ( ( G m ) − χ m ( s ))
|     |     |     |     |     |     |     |     |     | 2   | 2 b | est ,i,j i ,j |     |     |     |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | ------------- | --- | --- | --- |
suspiciouslyhighaccuracies.
|       |                        |             |     |        |                       |     |          |     | χm+1(s+1)=χm(s)+Vel |     |     | m+1 | (s+1) |     |
| ----- | ---------------------- | ----------- | --- | ------ | --------------------- | --- | -------- | --- | ------------------- | --- | --- | --- | ----- | --- |
| 3.4.2 | Earlystoppingmechanism |             |     |        |                       |     |          |     |                     |     | i,j |     |       | (5) |
|       |                        |             |     |        |                       |     |          |     | i,j                 |     |     | i,j |       |     |
|       | Training               | termination |     | occurs | when cross-validation |     | accuracy |     |                     |     |     |     |       |     |
m denotesthevelocityofthem-thdolphinindimension
| shows | no  | improvement |     | over 15 | consecutive | iterations, |     | with where,Vel | i,j |     |     |     |     |     |
| ----- | --- | ----------- | --- | ------- | ----------- | ----------- | --- | -------------- | --- | --- | --- | --- | --- | --- |
|       |     |             |     |         |             |             |     |                | χ   | m   |     |     | ℘ m |     |
improvementthresholdsetto0.001. j at iteration i, is the dolphin’s position, represents
|     |     |     |     |     |     |     |     |              |      | i ,j      |           |     | b est,i,j |             |
| --- | --- | --- | --- | --- | --- | --- | --- | ------------ | ---- | --------- | --------- | --- | --------- | ----------- |
|     |     |     |     |     |     |     |     | the personal | best | dolphin’s | position, | Gm  | is the    | global best |
best,i,j
positionamongtheswarm,A1,A2aretheaccelerationcoefficients,
3.4.3 Bootstrapvalidation ω istheinertiaweight,ν 1,ν aretherandomnumbersuniformly
2
|     | Foradditionalrobustnessassessment,weemployedbootstrap |     |     |     |     |     |     | distributedin[0,1]. |     |     |     |     |     |     |
| --- | ----------------------------------------------------- | --- | --- | --- | --- | --- | --- | ------------------- | --- | --- | --- | --- | --- | --- |
resampling(n=50)without-of-bagevaluationtoestimatemodel The dolphins attempt to catch up and then attack schools
|     |     |     |     |     |     |     |     | of sardines | at the | attack | phase, eventually |     | preying | on them. If |
| --- | --- | --- | --- | --- | --- | --- | --- | ----------- | ------ | ------ | ----------------- | --- | ------- | ----------- |
stabilityandgeneralizationcapability.
thesedolphinspursuetheschoolofsardinesforasignificanttime
interval,theycatchupwiththeswarmorreachfavorablepositions
3.5 DOA from where they can prey on the sardines. But based on our
assumptions,dolphinsonlytakearotaryswimmingposturewhile
attackingorpreyingonschoolsofsardines.Assumingthataherd
|     | DOA | is a new | method | that gets | its ideas | from | how dolphins |     |     |     |     |     |     |     |
| --- | --- | -------- | ------ | --------- | --------- | ---- | ------------ | --- | --- | --- | --- | --- | --- | --- |
<
communicate and hunt cooperatively (Wu et al., 2016; Al-Taie ofdolphinskeepspursuingsardinesoverdisjointtimestepss(s
Gen),someofthemreachfavorablepositionsfromwhichtheycan
| and | Khaleel, | 2024). | Because | it  | is a nature-inspired |     | practice, |     |     |     |     |     |     |     |
| --- | -------- | ------ | ------- | --- | -------------------- | --- | --------- | --- | --- | --- | --- | --- | --- | --- |
DOA has received noticeable attention since it works well attackorpreyontheherdofsardines.Duetothiscondition,their
positionsareupdatedbasedonEquations6,7.
| on        | complex       | issues       | in various    | areas.        | This    | algorithm  | capitalizes |     |     |     |     |     |     |     |
| --------- | ------------- | ------------ | ------------- | ------------- | ------- | ---------- | ----------- | --- | --- | --- | --- | --- | --- | --- |
| on        | the intrinsic |              | intelligence, | agility,      | and     | social     | behaviors   | of  |     |     |     |     |     |     |
| dolphins, |               | particularly | their         | collaborative | hunting | techniques |             | and |     |     |     |     |     |     |
+1(s+1)
|                                      |     |     |     |     |     |     |     |     |     | χ m  | =νQ |           |     | (6) |
| ------------------------------------ | --- | --- | --- | --- | --- | --- | --- | --- | --- | ---- | --- | --------- | --- | --- |
| sophisticatedcommunicationabilities. |     |     |     |     |     |     |     |     |     | i ,n |     | ,(cid:15) |     |     |
+1(s+1)
Some of the traits show that dolphins are recognized as one χ m =ν i,(cid:15) (7)
i ,n
ofthemostintelligentmarinespecies,suchasecho-location,team
cooperation, task allocation, and acoustic communication. These whereνQ ,(cid:15)andν i,(cid:15)aretherandomnumbersuniformlydistributed
distinctive traits have been effectively integrated into the DOA in [0,1],(cid:15) is the random dolphin index from the population,
allowing to look for new solutions and still improve existing s denotes the current time step, Gen represents the maximum
ones while managing to use resources wisely (Gholizadeh and numberofiterations.
Poorhoseini,2016). The final stage, known as switching swimming modes, the
DOA copies how dolphins in nature move together which dolphins transition into a dynamic swimming mode, allowing
allowsforbetternavigationthroughcomplicatedandwide-ranging themtoadoptvariousswimmingpositions.Duringthisphase,we
problem spaces (Soto et al., 2016; Yong et al., 2016). The main computethevalueof(Y)byapplyingthebelowequation:
| FrontiersinBigData |     |     |     |     |     |     |     | 07  |     |     |     |     |     | frontiersin.org |
| ------------------ | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --------------- |

RachelandDevarasan 10.3389/fdata.2025.1641714
1: Step 1: Initialization
1 2: Set the population size (pop_size=30), maximum
χk = , k=1,2,...,(cid:6) (8)
i,j 2·χk(s) iterations (max_iter=50)
i,j
3: Set parameters ω=0.7,a1,a2 =2.0
ThevaluesYjaresortedinascendingorder.IfO
k
(s)istherank
4: Set early stopping threshold = 15 iterations,
ofYjattimes,thenwecomputeϕ:
improvement threshold = 0.001
5: Step 2: Initialize the population of dolphins
ϕk(s)= O k (s)−1 (9) 6: for each dolphin i=1 to pop_size do
i,j (cid:6)
7: Initialize position randomly within bounds
where,(cid:6)denotesthetotalnumberofdolphins,O (s)istherankof [A:0.1−100,ϑ:0.001−1]
k
thek-thdolphin’ssolutionatiterationsandtheϕvaluedetermine 8: end for
the effectiveness of the switch (during strike or pursuit phase) 9: Step 3: Main optimization loop
(Sharma and Kaul, 2018; Qiao and Yang, 2019) as illustrated in 10: for iter=1 to max_iter do
Algorithm1. 11: for each dolphin i=1 to pop_size do
12: Calculate fitness by evaluating
classification quality
3.5.2 ProcedureforclassificationusingDOA
13: if fitness is better than personal best then
The classification process utilizes the DOA by the described 14: Update personal best position and fitness
procedure: 15: end if
16: if fitness is better than global best then
1. Start by initializing a collection of parameters, such as the
communitysize(cid:6),thecoefficientsψ,A1,,andA2,themaximum 17: Update global best position and fitness
18: Reset early stopping counter
number of iterations that Gen indicates, and the time steps
(τ =1)areindicatedbyψ. 19: end if
2. Generatetheinitialcommunityrandomly. 20: Generate random coefficients ν 1,ν 2
3. Correlation between all data matrices (train and test) and the 21: Calculate cognitive component = a1 · ν 1 ·
(personal_best−current_position)
target matrix is utilized, as shown by the following equation,
determinethefitnessfunctionforeachgroupmember: 22: Calculate social component = a2 · ν 2 ·
(global_best−current_position)
(cid:5) 23: Update position = current_position +
Fitness= (cid:6) (cid:5)
α,β(U
αβ
−U)
(cid:5)
(V
αβ
−V)
(10)
cognitive+social
α,β(U
αβ
−U)2× α,β(V
αβ
−V)2 24: Clip position to reasonable bounds
25: end for
wheretheimageandtargetmatricesofdimensions(α×β)are
26: if no improvement in global best then
indicatedbyUandV,respectively.
27: Increment early stopping counter
4. Thetimestepτ isincrementedbyone. 28: end if
5. Thefollowingstepsareincludedinthisstage: 29: if early stopping counter ≥ threshold then
30: Break
• Whenτ ≤(cid:18),thedolphinpositionsareupdatedaccording
31: end if
to Equations4, 5 and the new solutions are inspected. If 32: end for
they are better than the old ones, an update is made and 33: Step 4: Return best parameters (A,ϑ)
thebestsolutionisstored.
• Forthecasewhereτ >(cid:18),γ iscalculatedandthensorted Algorithm1. DOA.
inincreasingorderbyEquation8.Switchϕiscalculatedfor
eachdolphinbyEquation9.Randomvaluesarecalculated
between 0 and 1. If ϕ ≤ ζ, the positions are updated by 3.6 DOA-SVM hybrid
Equations6,7.Thenewsolutionsarecalculated,andifthey
perform better than their predecessors, they are updated, ML specifically supervised learning algorithms, is employed
thusthebestsolutionisselected. in bothregressionandclassification problems(Fayeetal.,2018).
Its effectiveness and high accuracy are particularly notable in
6. The stopping condition will be satisfied either by when the classificationtasks.InthecontextoftheSVMclassifier,theselection
solutionisachievedasdesiredorbywhenthenumberofcycles of optimal parameters is crucial for achieving high performance
predetermined has expired; otherwise, the process returns to in biometric identification systems (Qi et al., 2013; Shao et al.,
step4. 2011). In the DOA-SVM hybrid, we use DOA to optimize the
SVMparameters(Aandϑ forRBFkernel)whilesimultaneously
performingfeatureselection.Thefitnessfunctionisdefinedasthe
3.5.3 DOAimplementation classificationaccuracyonavalidationset.
Our standalone DOA implementation uses the following UsingtheDOAandSVMmethodssimultaneouslyprovidesa
approachasshowninAlgorithm1. modern approach to improve solving the classification problems
FrontiersinBigData 08 frontiersin.org

| RachelandDevarasan |            |     |                  |     |      |            |     |         |                   |     |     | 10.3389/fdata.2025.1641714 |     |
| ------------------ | ---------- | --- | ---------------- | --- | ---- | ---------- | --- | ------- | ----------------- | --- | --- | -------------------------- | --- |
| with the           | advantages | of  | both approaches. |     | Such | algorithms | can |         |                   |     |     |                            |     |
|                    |            |     |                  |     |      |            |     | 1: Step | 1: Initialization |     |     |                            |     |
interact and use their smartness together to enhance prediction 2: Set population size (pop_size = 30), maximum
accuracy and ensure the best possible outcomes which improves iterations (max_iter = 50)
|     |     |     |     |     |     |     |     |     |     |     | ω=0.7, | =a2 | =2.0 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | ------ | --- | ---- |
the model’s overall performance. Analyzing the specific elements 3: Set DOA parameters a1
andworkingsofthisapproachwhichprovidesbetterunderstanding 4: Define parameter bounds for SVM: A: [0.1−100], ϑ
of how the two algorithms can solve problems efficiently at the [0.001−1]
:
same time. Also, by looking into the problems and possible 5: Initialize early stopping threshold and counter
improvementsoftheseintegrationtechniques,thewaystoimprove 6: Step 2: Initialize the dolphin population
i=1
optimization and classification methods in the future can be 7: for each dolphin to pop_size do
determined,asdemonstratedinAlgorithm2. 8: Randomly initialize A and ϑ within the given
bounds
|     |     |     |     |     |     |     |     | 9: Evaluate |     | fitness | by  | training | an SVM with |
| --- | --- | --- | --- | --- | --- | --- | --- | ----------- | --- | ------- | --- | -------- | ----------- |
3.6.1 Procedureofclassificationutilizingahybrid parameters (A,ϑ)
technique =
|                                                              |                |              |          |            |           |           |         | 10: Compute |                 | accuracy | on validation |        | set (fitness   |
| ------------------------------------------------------------ | -------------- | ------------ | -------- | ---------- | --------- | --------- | ------- | ----------- | --------------- | -------- | ------------- | ------ | -------------- |
| The                                                          | classification | process      | with     | the hybrid | DOA-SVM   |           | model   | −1×         | accuracy)       |          |               |        |                |
| stick to the                                                 | general        | optimization |          | procedure  | described | in        | Section |             |                 |          |               |        |                |
|                                                              |                |              |          |            |           |           |         | 11: Store   | personal        |          | best and      | update | global best if |
| 3.4.2, utilizing                                             |                | the DOA.     | However, | it         | differs   | in the    | fitness | applicable  |                 |          |               |        |                |
| function,whichistailoredforSVM-basedclassificationasfollows: |                |              |          |            |           |           |         | 12: end     | for             |          |               |        |                |
| The current                                                  | solution,      | represented  |          | by the     | dolphin’s | position, | is      |             |                 |          |               |        |                |
|                                                              |                |              |          |            |           |           |         | 13: Step    | 3: Optimization |          | loop          |        |                |
translated into the parameters for the SVM—specifically, the 14: for iter =1 to max_iter do
penaltyparameterA,thelossfunction(cid:7),andthekernelparameter i=1
|     |     |     |     |     |     |     |     | 15: for | each | dolphin |     | to pop_size | do  |
| --- | --- | --- | --- | --- | --- | --- | --- | ------- | ---- | ------- | --- | ----------- | --- |
ϑ.AnSVMmodelisthentrainedusingthesemappedparameters
|     |     |     |     |     |     |     |     | 16: | Generate | random | values | ν 1,ν | ∈[0,1] |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | -------- | ------ | ------ | ----- | ------ |
2
along with the training dataset. Subsequently, the trained SVM 17: Compute cognitive component: a1 · ν ·
1
model is assessed with a separate test dataset. The fitness of the (personal_best−current_position)
currentdolphin(solution)iscalculatedasthesumofsquarederrors 18: Compute social component: a2 · ν ·
2
(SSE)betweenthepredictedlabelsandtheactuallabelsinthetest (global_best−current_position)
dataset.Thisfitnessvalueisthenemployedtosteertheoptimization 19: Update velocity and position using DOA
| process. |     |     |     |     |     |     |     | update | rule  |           |              |       |              |
| -------- | --- | --- | --- | --- | --- | --- | --- | ------ | ----- | --------- | ------------ | ----- | ------------ |
|          |     |     |     |     |     |     |     | 20:    | Clip  | position  | to parameter |       | bounds       |
|          |     |     |     |     |     |     |     | 21:    | Train | SVM using | updated      | (A,ϑ) | and evaluate |
3.6.2 DOA-SVMimplementation
fitness
| The                                                   | DOA-SVM | hybrid | approach | is presented |     | in the | form of |          |                |          |           |              |             |
| ----------------------------------------------------- | ------- | ------ | -------- | ------------ | --- | ------ | ------- | -------- | -------------- | -------- | --------- | ------------ | ----------- |
|                                                       |         |        |          |              |     |        |         | 22:      | Update         | personal | and       | global bests | accordingly |
| pseudocodeinAlgorithm2.                               |         |        |          |              |     |        |         | 23: end  | for            |          |           |              |             |
|                                                       |         |        |          |              |     |        |         | 24: if   | no improvement |          | in global | best         | then        |
|                                                       |         |        |          |              |     |        |         | 25:      | Increment      | early    | stopping  | counter      |             |
| 3.7 FCM-DOA-SVM                                       |         |        | hybrid   |              |     |        |         |          |                |          |           |              |             |
|                                                       |         |        |          |              |     |        |         | 26: else |                |          |           |              |             |
|                                                       |         |        |          |              |     |        |         | 27:      | Reset          | early    | stopping  | counter      |             |
| ClusteringalgorithmssuchasFCMareextensivelyemployedin |         |        |          |              |     |        |         | 28: end  | if             |          |           |              |             |
preprocessing tasks toidentify significant patterns andstructures ≥
|              |     |               |        |         |            |           |     | 29: if | early | stopping | counter | threshold | then |
| ------------ | --- | ------------- | ------ | ------- | ---------- | --------- | --- | ------ | ----- | -------- | ------- | --------- | ---- |
| within data. | FCM | is especially | useful | in soft | clustering | contexts, |     | 30:    | Break |          |         |           |      |
whereeachdatapointcanbeassociatedwithmultipleclustersto
|                 |         |     |                |       |           |     |         | 31: end | if  |     |     |     |     |
| --------------- | ------- | --- | -------------- | ----- | --------- | --- | ------- | ------- | --- | --- | --- | --- | --- |
| varying extents | (Mendel |     | and Bonissone, | 2021; | Chimatapu |     | et al., |         |     |     |     |     |     |
|                 |         |     |                |       |           |     |         | 32: end | for |     |     |     |     |
2018). This capability is advantageous in biometric applications, 33: Step 4: Final training and evaluation
wheredatafrequentlydisplaysoverlappingfeatures.IntheFCM- (A,ϑ)
|     |     |     |     |     |     |     |     | 34: Train | SVM | with | best parameters |     | on full |
| --- | --- | --- | --- | --- | --- | --- | --- | --------- | --- | ---- | --------------- | --- | ------- |
DOA-SVM hybrid model, the FCM algorithm improves the training data
| input features | by             | adding    | fuzzy | membership | values      | to each | data     |             |                |     |           |     |     |
| -------------- | -------------- | --------- | ----- | ---------- | ----------- | ------- | -------- | ----------- | -------------- | --- | --------- | --- | --- |
|                |                |           |       |            |             |         |          | 35: Predict | labels         | on  | test data |     |     |
| sample,        | thus capturing | essential |       | structural | information |         | prior to |             |                |     |           |     |     |
|                |                |           |       |            |             |         |          | 36: Compute | classification |     | metrics   |     |     |
classification(Shuklaetal.,2020;Ferreyraetal.,2019). 37: return best parameters and evaluation results
Subsequently,theDOAisutilizedtofine-tunetheparameters
oftheSVMclassifier,specifically(AandϑfortheRBFkernel).This
|            |        |             |            |                  |     |            |         | Algorithm2. | DOA-SVMhybridclassificationalgorithm. |     |     |     |     |
| ---------- | ------ | ----------- | ---------- | ---------------- | --- | ---------- | ------- | ----------- | ------------------------------------- | --- | --- | --- | --- |
| integrated | method | effectively | merges     | the unsupervised |     | clustering |         |             |                                       |     |     |     |     |
| capability | of FCM | with the    | supervised | learning         | of  | SVM        | and the |             |                                       |     |     |     |     |
globaloptimizationpotentialofDOA.Inthisscenario,thefitness
functionischaracterizedbytheclassificationaccuracyachievedon
avalidationsetusingtheSVMtrainedwithfeaturesenhancedby representation, and DOA facilitates the exploration of optimal
FCM. classifier configurations. Consequently, the model demonstrates
The FCM-DOA-SVM architecture exhibits a significant enhancedgeneralizationandrobustnessinbiometricclassification
synergistic effect, wherein fuzzy memberships enhance feature tasks,asdetailedinAlgorithm3.
| FrontiersinBigData |     |     |     |     |     |     |     | 09  |     |     |     |     | frontiersin.org |
| ------------------ | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --------------- |

RachelandDevarasan 10.3389/fdata.2025.1641714
3.7.1 Procedureofclassificationutilizingthe 1: Step 1: Initialization
FCM-DOA-SVMhybrid 2: Set FCM parameters: number of clusters
The classification procedure utilizing the FCM-DOA-SVM (n_clusters = 3), fuzziness parameter m=2.0
modelisanextensionofthegeneraloptimizationstrategyoutlined 3: Set maximum iterations (max_iter = 100),
in Section 3.5.1, incorporating FCM-based feature enhancement convergence threshold (epsilon = 1e-4)
intotheprocess.Initially,thetrainingdataissubjectedtoclustering 4: Set DOA parameters: population size (pop_size =
throughtheFCMalgorithm,resultinginamembershipmatrixthat 30), max iterations (max_iter = 50)
indicatesthedegreeofassociationofeachsamplewitheachcluster. 5: Set ω=0.7, a1 =a2 =2.0
These membership values are then integrated with the original 6: Define parameter bounds: A:[0.1−100],
featuresettocreateanenrichedfeaturematrix. ϑ:[0.001−1]
In the subsequent phase, each dolphin (solution) within 7: Initialize early stopping threshold and counter
the DOA population signifies a potential configuration of SVM 8: Step 2: FCM-Based Feature Enhancement
hyperparameters: specifically, the penalty parameter A and the 9: Train FCM on input data χ train
kernel parameter ϑ. For each solution, an SVM is trained 10: Compute fuzzy membership matrix Utrain
utilizing the enriched feature matrix and subsequently evaluated 11: Concatenate original features and membership
on a validation set. The fitness of a dolphin is determined by values: χ
t
e
r
n
a
h
in
=[χ train,Utrain]
the classification accuracy, which directs the evolution of the 12: Step 3: Initialize Dolphin Population
populationtowardmoreoptimalparametersets. 13: for each dolphin i=1 to pop_size do
Thishybridizationenablesacomprehensivelearningprocess: 14: Randomly initialize A and ϑ within bounds
FCM identifies fuzzy patterns within the data, DOA effectively 15: Train SVM with (A,ϑ) on χenh
train
explores the parameter space, and SVM executes the final 16: Evaluate performance on validation data
classification. Collectively, these components produce a highly (fitness =−1× accuracy)
accurate and interpretable classification model, well-suited for 17: Update personal best and global best positions
complexbiometricrecognitiontasks. 18: end for
19: Step 4: Optimization Loop
20: for iter =1 to max_iter do
3.7.2 FCM-DOA-SVMimplementation 21: for each dolphin i=1 to pop_size do
ThepseudocodeforthehybridapproachFCM-DOA-SVMis 22: Generate random values ν 1,ν 2 ∈[0,1]
giveninAlgorithm3. 23: Compute cognitive component: a1 · ν 1 ·
(personal_best−current_position)
24: Compute social component: a2 · ν 2 ·
3.8 NCM-DOA-SVM hybrid
(global_best−current_position)
25: Update velocity and position using DOA rules
NCM-DOA-SVM hybrid includes the NCM method of 26: Clip position to parameter bounds
clustering which is helpful for CLFP recognition to handle 27: Train SVM with updated parameters on χenh
train
indeterminacy in images. Neutrosophic Theory represents the 28: Evaluate fitness and update personal/global
information that is uncertain, imprecise and inconsistent by bests
using three degrees, truth (T), indeterminacy (I) and falsity 29: end for
(F). The NCM clustering algorithm extends the classical FCM 30: if no improvement in global best then
by incorporating this neutrosophic concept, enabling more 31: Increment early stopping counter
accurate modeling of real-world biometric data. In the proposed 32: else
NCM-DOA-SVM hybrid, T, I and F are extracted for every 33: Reset early stopping counter
sample during the clustering procedure which enhance feature 34: end if
representations. 35: if early stopping counter ≥ threshold then
Subsequently, the DOA algorithm is used to improve the 36: Break
hyperparametersoftheSVMclassifier,specificallythepenaltyvalue 37: end if
(A) and the RBF kernel value (ϑ). Using this hybrid approach 38: end for
boosts the classifier’s results by finding the optimal parameters 39: Step 5: Final Training and Evaluation
andadditionallymakinguseoftheextrafeaturesthatcapturethe 40: Train final FCM on full training data
neutrosophicnatureoftheinputdata. 41: Compute Utest and create χ
t
e
e
n
s
h
t
=[χ test,Utest]
The integration of NCM for handling unclear data, DOA 42: Train SVM with best parameters on χenh
train
for finding global solution and SVM for accurate classification 43: Predict labels on χenh
test
leads to the development of a strong model for biometric 44: Compute classification metrics
recognition. By applying the combined model, it deals with 45: return best parameters and evaluation results
uncertainty and the classifier becomes more effective as shown
inAlgorithm4. Algorithm3. FCM-DOA-SVMhybridclassificationalgorithm.
FrontiersinBigData 10 frontiersin.org

| RachelandDevarasan |     |     |     |     |     |     |     |     |     |     |     | 10.3389/fdata.2025.1641714 |     |     |
| ------------------ | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | -------------------------- | --- | --- |
3.8.1 Procedureofclassificationutilizingthe
|     |     |     |     |     |     |     | 1: Step | 1: Initialization |     |     |     |     |     |     |
| --- | --- | --- | --- | --- | --- | --- | ------- | ----------------- | --- | --- | --- | --- | --- | --- |
NCM-DOA-SVMhybrid
|     |     |     |     |     |     |     | 2: Set | NCM | parameters: |     | number |     | of  | clusters |
| --- | --- | --- | --- | --- | --- | --- | ------ | --- | ----------- | --- | ------ | --- | --- | -------- |
The classification procedure of the NCM-DOA-SVM hybrid (n_clusters = 3), fuzziness parameter m=2.0
model adheres to the core optimization approach introduced in 3: Set maximum iterations (max_iter = 100),
Section 3.5.1. Initially, the input dataset undergoes unsupervised convergence threshold (epsilon = 1e-4)
| clustering | using | the NCM | algorithm. | This | step computes |     | the    |                 |     |            |     |      |           |     |
| ---------- | ----- | ------- | ---------- | ---- | ------------- | --- | ------ | --------------- | --- | ---------- | --- | ---- | --------- | --- |
|            |       |         |            |      |               |     | 4: Set | DOA parameters: |     | population |     | size | (pop_size | =   |
truth,indeterminacy,andfalsitymembershipsforeachdatapoint, 30), max iterations (max_iter = 50)
resultinginanenhancedfeaturespaceoftheform[X,T,I,F]. 5: Set ω=0.7, a1 =a2 =2.0
A:[0.1−100],
Each solution (dolphin) in the population corresponds to a 6: Define parameter bounds:
candidatesetofSVMparameters,includingthepenaltycoefficient ϑ:[0.001−1]
| A and the | kernel | function | parameter | ϑ. The | SVM | classifier |               |     |       |          |           |     |     |         |
| --------- | ------ | -------- | --------- | ------ | --- | ---------- | ------------- | --- | ----- | -------- | --------- | --- | --- | ------- |
|           |        |          |           |        |     |            | 7: Initialize |     | early | stopping | threshold |     | and | counter |
is trained on the NCM-enhanced features using each dolphin’s 8: Step 2: Neutrosophic Feature Enhancement
parameters and evaluated on a validation dataset. The dolphin’s 9: Train NCM on input data X
train
fitnessiscomputedbasedontheclassificationperformance,such 10: Compute T), I, and F membership matrices
astheSSEorclassificationaccuracy. 11: Concatenate original features and neutrosophic
The optimization process iteratively refines the solutions Xe n h =[X
|     |     |     |     |     |     |     | memberships: |     | t r ain |     | train,T,I,F] |     |     |     |
| --- | --- | --- | --- | --- | --- | --- | ------------ | --- | ------- | --- | ------------ | --- | --- | --- |
based on fitness, guiding the swarm toward the best SVM 12: Step 3: Initialize Dolphin Population
configuration.ThiscollaborativemechanismamongNCM,DOA, 13: for each dolphin i=1 to pop_size do
ϑ
and SVM not only improves classification precision but also 14: Randomly initialize A and within bounds
ensuresrobustnesstonoisyanduncertainbiometricdata.Thefinal 15: Train SVM with (A,ϑ) on Xenh
train
| model thus | integrates | fuzzy | reasoning, | optimization |     | intelligence, |              |     |             |     |     |            |     |      |
| ---------- | ---------- | ----- | ---------- | ------------ | --- | ------------- | ------------ | --- | ----------- | --- | --- | ---------- | --- | ---- |
|            |            |       |            |              |     |               | 16: Evaluate |     | performance |     | on  | validation |     | data |
anddiscriminativelearningtodeliversuperiorperformance. (fitness =−1× accuracy)
|     |     |     |     |     |     |     | 17: Update | personal |     | best | and | global | best positions |     |
| --- | --- | --- | --- | --- | --- | --- | ---------- | -------- | --- | ---- | --- | ------ | -------------- | --- |
|     |     |     |     |     |     |     | 18: end    | for      |     |      |     |        |                |     |
3.8.2 NCM-DOA-SVMimplementation 19: Step 4: Optimization Loop
| TheNCM-DOA-SVMhybridapproachispresentedintheform |     |     |     |     |     |     |         | =1   |     |          |     |     |     |     |
| ------------------------------------------------ | --- | --- | --- | --- | --- | --- | ------- | ---- | --- | -------- | --- | --- | --- | --- |
|                                                  |     |     |     |     |     |     | 20: for | iter | to  | max_iter | do  |     |     |     |
ofpseudocodeinAlgorithm4. 21: for each dolphin i=1 to pop_size do
|     |     |     |     |     |     |     | 22: | Generate | random | values |     | ν 1,ν ∈[0,1] |     |     |
| --- | --- | --- | --- | --- | --- | --- | --- | -------- | ------ | ------ | --- | ------------ | --- | --- |
2
|           |     |            |     |     |     |     |     |         |           |     |     |            |     | · ν · |
| --------- | --- | ---------- | --- | --- | --- | --- | --- | ------- | --------- | --- | --- | ---------- | --- | ----- |
|           |     |            |     |     |     |     | 23: | Compute | cognitive |     |     | component: |     | a1 1  |
| 4 Results | and | discussion |     |     |     |     |     |         |           |     |     |            |     |       |
(personal_best−current_position)
|     |     |     |     |     |     |     |     |     |     |     |     |     |     | · ν · |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | ----- |
4.1 Algorithm training and validation 24: Compute social component: a2 2
(global_best−current_position)
phases
|     |     |     |     |     |     |     | 25: | Update | velocity | and | position  | using  | DOA | rules |
| --- | --- | --- | --- | --- | --- | --- | --- | ------ | -------- | --- | --------- | ------ | --- | ----- |
|     |     |     |     |     |     |     | 26: | Clip   | position | to  | parameter | bounds |     |       |
Atthisstage,adatasetconsistingof2016CLFPimageswasused 27: Train SVM with updated parameters on Xenh
train
todeveloptheDOAandthesuggestedhybridapproaches,namely
|          |              |     |     |              |     |         | 28: | Evaluate | fitness |     | and update | personal/global |     |     |
| -------- | ------------ | --- | --- | ------------ | --- | ------- | --- | -------- | ------- | --- | ---------- | --------------- | --- | --- |
| DOA-SVM, | FCM-DOA-SVM, |     | and | NCM-DOA-SVM. |     | The HOG |     |          |         |     |            |                 |     |     |
bests
| techniquewasusedforinitialimageprocessingandtraitextraction, |     |     |     |     |     |     | 29: end | for |     |     |     |     |     |     |
| ------------------------------------------------------------ | --- | --- | --- | --- | --- | --- | ------- | --- | --- | --- | --- | --- | --- | --- |
whichmadeiteasiertoextractafeaturematrixfromtheseimages.
|     |     |     |     |     |     |     | 30: if | no improvement |     | in  | global | best | then |     |
| --- | --- | --- | --- | --- | --- | --- | ------ | -------------- | --- | --- | ------ | ---- | ---- | --- |
TheDOAandthesuggestedhybridapproaches(DOA-SVM,FCM- 31: Increment early stopping counter
| DOA-SVM, | and | FCM-DOA-SVM) |     | were then | trained | using | the      |     |     |     |     |     |     |     |
| -------- | --- | ------------ | --- | --------- | ------- | ----- | -------- | --- | --- | --- | --- | --- | --- | --- |
|          |     |              |     |           |         |       | 32: else |     |     |     |     |     |     |     |
derivedtraitsmatrix,improvingtheirperformance.
|      |                |     |        |                |     |           | 33:     | Reset | early | stopping | counter |     |     |     |
| ---- | -------------- | --- | ------ | -------------- | --- | --------- | ------- | ----- | ----- | -------- | ------- | --- | --- | --- |
| Upon | the completion |     | of the | training phase | and | achieving | 34: end | if    |       |          |         |     |     |     |
algorithm stability, a test was conducted involving 504 CLFP ≥
|               |            |         |            |                   |              |           | 35: if  | early | stopping | counter |     | threshold |     | then |
| ------------- | ---------- | ------- | ---------- | ----------------- | ------------ | --------- | ------- | ----- | -------- | ------- | --- | --------- | --- | ---- |
| images. These | images     | were    | subjected  | to preprocessing, |              | followed  | 36:     | Break |          |         |     |           |     |      |
| by feature    | extraction | using   | the        | HOG algorithm.    | To           | determine |         |       |          |         |     |           |     |      |
|               |            |         |            |                   |              |           | 37: end | if    |          |         |     |           |     |      |
| the optimal   | HOG        | feature | extraction | parameters,       | we conducted |           | a       |       |          |         |     |           |     |      |
|               |            |         |            |                   |              |           | 38: end | for   |          |         |     |           |     |      |
comprehensive sensitivity analysis across different cell and block 39: Step 5: Final Training and Evaluation
sizeconfigurations.
|     |     |     |     |     |     |     | 40: Train | final | NCM | on full | training | data |     |     |
| --- | --- | --- | --- | --- | --- | --- | --------- | ----- | --- | ------- | -------- | ---- | --- | --- |
AsshowninTable3,threeconfigurationswereevaluated:Fine 41: Compute T, I, F memberships for test data
( 1 6 × 1 6 c e ll s , 2 × 2 b lo c k s) ,M e di u m (3 2 × 3 2 c e l ls , 2 × 2 b lo c k s ) , X e n h = [X
|              |             |               |         |                 |                |          | 4 2 : C r e a | t e t e | s t      | t e s t, T | , I , F ]    |     |      |     |
| ------------ | ----------- | ------------- | ------- | --------------- | -------------- | -------- | ------------- | ------- | -------- | ---------- | ------------ | --- | ---- | --- |
| a n d Co a r | s e ( 6 4 × | 6 4 c e ll s, | 2 × 2 b | l ock s) . Th e | r e s u l ts d | em o n s | tr a t e      |         |          |            |              |     |      |     |
|              |             |               |         |                 |                |          | 4 3 : T r a i | n S VM  | w i th b | e s t p a  | r a m e ters | on  | Xenh |     |
train
a clear trade-off between feature dimensionality, computational 44: Predict labels on Xenh
test
efficiency,andclassificationaccuracy.TheMediumconfiguration
|     |     |     |     |     |     |     | 45: Compute | classification |     |     | metrics |     |     |     |
| --- | --- | --- | --- | --- | --- | --- | ----------- | -------------- | --- | --- | ------- | --- | --- | --- |
achieved the highest accuracy of 0.856198, despite having a 46: return best parameters and evaluation results
| moderate           | feature        | vector | size of  | 324 dimensions. | Interestingly, |         |             |                                           |     |     |     |     |                 |     |
| ------------------ | -------------- | ------ | -------- | --------------- | -------------- | ------- | ----------- | ----------------------------------------- | --- | --- | --- | --- | --------------- | --- |
| the Fine           | configuration, |        | with its | substantially   | larger         | feature |             |                                           |     |     |     |     |                 |     |
|                    |                |        |          |                 |                |         | Algorithm4. | NCM-DOA-SVMhybridclassificationalgorithm. |     |     |     |     |                 |     |
| FrontiersinBigData |                |        |          |                 |                |         | 11          |                                           |     |     |     |     | frontiersin.org |     |

| RachelandDevarasan |     |     |     |     |     |     |     |     |     |     |     | 10.3389/fdata.2025.1641714 |     |     |
| ------------------ | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | -------------------------- | --- | --- |
TABLE3 HOGparametersensitivityanalysisresults.
Configuration Cellsize Blocksize Accuracy Computationalefficiency
| Fine   |     |     | 16×16 |     |     | 2×2 |     | 0.844628 |     |     |     | 0.000567 |     |     |
| ------ | --- | --- | ----- | --- | --- | --- | --- | -------- | --- | --- | --- | -------- | --- | --- |
|        |     |     | 32×32 |     |     | 2×2 |     |          |     |     |     |          |     |     |
| Medium |     |     |       |     |     |     |     | 0.856198 |     |     |     | 0.003086 |     |     |
|        |     |     | 64×64 |     |     | 2×2 |     |          |     |     |     |          |     |     |
| Coarse |     |     |       |     |     |     |     | 0.842975 |     |     |     | 0.027778 |     |     |
TABLE4 TunedParametersfortheproposedmodel. TABLE5 OutcomeanalysisofsystemtestingwithDOA.
| Hyperparameter           |     |     |     |                  | Choice       |     |     | Exp.No. |     | Accuracy(%) |     | Processingtime(s) |           |     |
| ------------------------ | --- | --- | --- | ---------------- | ------------ | --- | --- | ------- | --- | ----------- | --- | ----------------- | --------- | --- |
| Datasetsplitting         |     |     |     | 75%train,25%test |              |     |     | 1       |     | 91.0        |     |                   | 6,110.23  |     |
| Classcount               |     |     |     |                  | 50           |     |     | 2       |     | 91.0        |     |                   | 2,109.28  |     |
| Initialdolphinpopulation |     |     |     |                  | 30           |     |     | 3       |     | 91.0        |     |                   | 2,091.07  |     |
| Earlystoppingthreshold   |     |     |     |                  | 15iterations |     |     | 4       |     | 91.0        |     |                   | 2,118.55  |     |
| Cross-validationfolds    |     |     |     |                  | 5            |     |     | 5       |     | 91.0        |     |                   | 2,091.81  |     |
| Bootstrapiterations      |     |     |     |                  | 50           |     |     | 6       |     | 91.0        |     |                   | 2,048.49  |     |
| Iterationthreshold       |     |     |     |                  | 50           |     |     | 7       |     | 91.0        |     |                   | 17,105.92 |     |
128×128
| Imageresolution |     |     |                                      |              |     |     |     | 8   |     | 91.0 |     |     | 1,663.03 |     |
| --------------- | --- | --- | ------------------------------------ | ------------ | --- | --- | --- | --- | --- | ---- | --- | --- | -------- | --- |
| HOG—block-size  |     |     |                                      |              | 2   |     |     | 9   |     | 91.0 |     |     | 1,954.80 |     |
| HOG—cell-size   |     |     |                                      |              | 32  |     |     | 10  |     | 91.0 |     |     | 1,732.15 |     |
| A1              |     |     | 2×(1−iteration(I)/max_iteration(MI)) |              |     |     |     |     |     |      |     |     |          |     |
| A2              |     |     |                                      | −1+I×(−1/MI) |     |     |     |     |     |      |     |     |          |     |
ψ 2×(1−I/MI) Thedatasetinclude504images,whichwasassessedusingthe
|     |     |     |     |     |     |     |     | DOA system | to determine | CLFP. | Table5 | provides |     | details on the |
| --- | --- | --- | --- | --- | --- | --- | --- | ---------- | ------------ | ----- | ------ | -------- | --- | -------------- |
trainingresultsoftheproposedalgorithm.
|               |                |     |             |       |               |             |          | The         | results of   | the test phase | of             | DOA are | detailed          | in Table5,     |
| ------------- | -------------- | --- | ----------- | ----- | ------------- | ----------- | -------- | ----------- | ------------ | -------------- | -------------- | ------- | ----------------- | -------------- |
| vector (1,764 | dimensions),   |     | yielded     | lower | accuracy      | (0.844628), |          |             |              |                |                |         |                   |                |
|               |                |     |             |       |               |             |          | which looks | at different | configurations |                | of      | the coefficients: | cell-          |
| suggesting    | that excessive |     | granularity |       | may introduce |             | noise or |             |              |                |                |         |                   |                |
|               |                |     |             |       |               |             |          | size HOG    | is always    | set at         | 32, block-size |         | HOG               | is fixed at 2, |
lead to overfitting. The negative correlation between feature dolphinpopulationlevelremainsat30,anditerationthresholdis
| size and | accuracy | (r = | −0.2498) | supports |     | this observation. |     |     |     |     |     |     |     |     |
| -------- | -------- | ---- | -------- | -------- | --- | ----------------- | --- | --- | --- | --- | --- | --- | --- | --- |
investigatedat50.
StatisticalanalysisusingANOVArevealednosignificantdifference The DOA-SVM system was employed to train a dataset
| between configurations |     | (p  | ≥ 0.05), | indicating |     | that the | Medium |            |       |             |     |         |         |         |
| ---------------------- | --- | --- | -------- | ---------- | --- | -------- | ------ | ---------- | ----- | ----------- | --- | ------- | ------- | ------- |
|                        |     |     |          |            |     |          |        | comprising | 1,512 | CLFP images |     | for the | purpose | of CLFP |
configurationprovidesanoptimalbalancebetweencomputational
identification.Theperformanceoftheproposedhybridalgorithm
efficiency (0.003086) and classification performance. The choice was evaluated using established assessment criteria, yielding the
| of 32 × | 32 cell sizes, | resulting |     | in four | cells for | a 128 | × 128 |     |     |     |     |     |     |     |
| ------- | -------------- | --------- | --- | ------- | --------- | ----- | ----- | --- | --- | --- | --- | --- | --- | --- |
followingresults:
| image, represents |             | a strategic | compromise |             | that captures | sufficient    |     |     |     |     |     |     |     |     |
| ----------------- | ----------- | ----------- | ---------- | ----------- | ------------- | ------------- | --- | --- | --- | --- | --- | --- | --- | --- |
| local gradient    | information |             | while      | maintaining |               | computational |     |     |     |     |     |     |     |     |
tractabilityandavoidingthecurseofdimensionalityassociatedwith
|     |     |     |     |     |     |     |     | A =91.2±1.8%, |     | R   | =90.7±2.0%,S |     | =91.6±1.6% |     |
| --- | --- | --- | --- | --- | --- | --- | --- | ------------- | --- | --- | ------------ | --- | ---------- | --- |
|     |     |     |     |     |     |     |     | CV            |     | CV  |              |     | CV         |     |
finergranularities.
|                |        |             |              |       |                 |            |     | BootstrapCI=[89.4%,93.0%], |     |     | Convergence=28iterations |     |     |     |
| -------------- | ------ | ----------- | ------------ | ----- | --------------- | ---------- | --- | -------------------------- | --- | --- | ------------------------ | --- | --- | --- |
| The assignment |        | of specific | coefficients |       | to each         | algorithm  |     | is                         |     |     |                          |     |     |     |
| determined     | by the | nature      | of the       | task, | as the specific | operations |     |                            |     |     |                          |     |     |     |
mayvarybasedontheresearchproblembeingaddressed.Through TheDOA-SVMsystemwasusedtoanalyzethedataset,which
| a series | of practical | experiments |     | and | their | application, | the |     |     |     |     |     |     |     |
| -------- | ------------ | ----------- | --- | --- | ----- | ------------ | --- | --- | --- | --- | --- | --- | --- | --- |
included504images,inordertofindCLFP.InTable6,theresults
appropriate coefficients for the algorithms were established. The arepresentedindetail.
valuesofthesecoefficientsaredetailedinTable4.
TheresultsofthetestphaseofthehybridmethodDOA-SVM,
Thedatasetwhichincluded1,512images,wasprocessedusing whichexaminesdifferentcoefficientconfigurations,aredetailedin
the DOA method to identify CLFP. The following results were Table6.HOGwithablock-sizefixedat2,adolphinpopulationlevel
obtainedfromtheevaluationofthesuggestedalgorithm’straining
heldconstantat30,aniterationthresholdexaminedat50,anda
usingpredeterminedevaluationcriteria: cell-sizecontinuouslymaintainedat32.
|     |     |     |     |     |     |     |     | The | FCM-DOA-SVM | system | was | trained | to  | identify CLFP |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | ----------- | ------ | --- | ------- | --- | ------------- |
usingadatasetcomprising1,512images.Theeffectivenessofthe
A =87.3±2.1%, R =86.8±2.3%,S =87.7±1.9%, proposed algorithm was assessed using evaluation metrics and
| CV  |     | CV  |     |     | CV  |     |     |     |     |     |     |     |     |     |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
BootstrapCI=[85.2%,89.4%], Convergence=32iterations theseareitsoutcomes:
| FrontiersinBigData |     |     |     |     |     |     |     | 12  |     |     |     |     |     | frontiersin.org |
| ------------------ | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --------------- |

| RachelandDevarasan |     |     |     |     |     |     |     |     |     | 10.3389/fdata.2025.1641714 |     |     |
| ------------------ | --- | --- | --- | --- | --- | --- | --- | --- | --- | -------------------------- | --- | --- |
TABLE6 OutcomeanalysisofsystemtestingwithDOA-SVM. TABLE8 OutcomeanalysisofsystemtestingwithNCM-DOA-SVM.
Exp.No. Accuracy(%) Processingtime(s) Exp.No. Accuracy(%) Processingtime(s)
| 1   |     | 94.0  |     | 1,282.58  |     | 1   |     | 98.0 |     |     | 1,341.57 |     |
| --- | --- | ----- | --- | --------- | --- | --- | --- | ---- | --- | --- | -------- | --- |
|     |     |       |     |           |     | 2   |     | 98.0 |     |     | 1,351.05 |     |
| 2   |     | 94.0  |     | 1,387.30  |     |     |     |      |     |     |          |     |
| 3   |     | 94.0  |     | 1,905.93  |     | 3   |     | 98.0 |     |     | 1,348.48 |     |
| 4   |     | 94.0  |     | 37,772.78 |     | 4   |     | 98.0 |     |     | 1,339.74 |     |
| 5   |     | 94.0  |     | 1,317.73  |     | 5   |     | 98.0 |     |     | 1,357.74 |     |
| 6   |     | 94.0  |     | 1,349.33  |     | 6   |     | 98.0 |     |     | 1,345.81 |     |
| 7   |     | 94.58 |     | 1,350.67  |     | 7   |     | 98.0 |     |     | 1,348.91 |     |
| 8   |     | 94.0  |     | 1,335.51  |     | 8   |     | 98.0 |     |     | 1,318.66 |     |
| 9   |     | 94.17 |     | 1,347.48  |     | 9   |     | 98.0 |     |     | 1,389.36 |     |
|     |     |       |     |           |     | 10  |     | 98.0 |     |     | 1,378.92 |     |
| 10  |     | 94.0  |     | 1,348.37  |     |     |     |      |     |     |          |     |
TABLE7 OutcomeanalysisofsystemtestingwithFCM-DOA-SVM.
|         |     |             |                   |          |     | TABLE9 Comprehensiveperformancecomparisonofproposedmethods. |     |            |     |          |     |            |
| ------- | --- | ----------- | ----------------- | -------- | --- | ----------------------------------------------------------- | --- | ---------- | --- | -------- | --- | ---------- |
| Exp.No. |     | Accuracy(%) | Processingtime(s) |          |     |                                                             |     |            |     |          |     |            |
|         |     |             |                   |          |     | Method                                                      |     | Numberof   |     | Accuracy |     | Processing |
|         |     |             |                   |          |     |                                                             |     | testimages |     | (%)      |     | time(s)    |
| 1       |     | 96.0        |                   | 1,388.09 |     |                                                             |     |            |     |          |     |            |
|         |     |             |                   |          |     | DOA                                                         |     | 504        |     | 91.00    |     | 3,902.533  |
| 2       |     | 96.26       |                   | 1,369.73 |     |                                                             |     |            |     |          |     |            |
|         |     |             |                   |          |     | DOA-SVM                                                     |     | 504        |     | 94.07    |     | 5,039.768  |
| 3       |     | 96.0        |                   | 1,370.13 |     |                                                             |     |            |     |          |     |            |
|         |     |             |                   |          |     | FCM-DOA-                                                    |     | 504        |     | 96.03    |     | 1,684.920  |
| 4       |     | 96.0        |                   | 4,614.75 |     |                                                             |     |            |     |          |     |            |
SVM
| 5   |     | 96.0 |     | 1,323.98 |     |          |     |     |     |       |     |           |
| --- | --- | ---- | --- | -------- | --- | -------- | --- | --- | --- | ----- | --- | --------- |
|     |     |      |     |          |     | NCM-DOA- |     | 504 |     | 98.00 |     | 1,352.024 |
SVM
| 6   |     | 96.0 |     | 1,344.68 |     |             |     |         |             |            |     |                |
| --- | --- | ---- | --- | -------- | --- | ----------- | --- | ------- | ----------- | ---------- | --- | -------------- |
| 7   |     | 96.0 |     | 1,357.67 |     |             |     |         |             |            |     |                |
| 8   |     | 96.0 |     | 1,387.29 |     |             |     |         |             |            |     |                |
|     |     |      |     |          |     | NCM-DOA-SVM |     | system: | The dolphin | population |     | level was kept |
| 9   |     | 96.0 |     | 1,369.75 |     |             |     |         |             |            |     |                |
at30,theiterationthresholdwasinvestigatedat50,andtheHOG
10 96.0 1,323.13 cell-size was continuously set at 32 and the HOG block-size was
fixedat2.
TheratingratiorateisshowninTable9fortheDOAmethod,
aswellasfortheproposedhybridapproaches:DOA-SVM,FCM-
| A =93.8±1.5%,              |     | R   | =93.3±1.7%,              | S =94.2±1.3% |     |                         |     |     |     |     |     |     |
| -------------------------- | --- | --- | ------------------------ | ------------ | --- | ----------------------- | --- | --- | --- | --- | --- | --- |
| CV                         |     | CV  |                          | CV           |     | DOA-SVM,andNCM-DOA-SVM. |     |     |     |     |     |     |
| BootstrapCI=[92.3%,95.3%], |     |     | Convergence=25iterations |              |     |                         |     |     |     |     |     |     |
ThedatawasassessedutilizingtheFCM-DOA-SVMsystemto
|     |     |     |     |     |     | 4.2 Experimental |     |     | analysis |     |     |     |
| --- | --- | --- | --- | --- | --- | ---------------- | --- | --- | -------- | --- | --- | --- |
identifyCLFP,comprising504images.Theoutputofthealgorithm
| training is | shown | in Table7 | (i.e., the test | phase | results of | the              |     |         |           |     |        |             |
| ----------- | ----- | --------- | --------------- | ----- | ---------- | ---------------- | --- | ------- | --------- | --- | ------ | ----------- |
|             |       |           |                 |       |            | The experimental |     | results | confirmed | the | robust | performance |
proposedhybridmethodareshowninthistable).TheFCM-DOA-
|           |          |           |            |            |      | of all proposed | algorithms |        | during | the training | phase.     | The best    |
| --------- | -------- | --------- | ---------- | ---------- | ---- | --------------- | ---------- | ------ | ------ | ------------ | ---------- | ----------- |
| SVM setup | kept HOG | cell-size | at 32, HOG | block-size | at 2 | and             |            |        |        |              |            |             |
|           |          |           |            |            |      | classification  | accuracy,  | 91.0%, | was    | found        | for images | with a size |
Dolphinpopulationat30anditvariedtheIterationthresholdfrom
|     |     |     |     |     |     | ×      |         |             |     | ×      |          |            |
| --- | --- | --- | --- | --- | --- | ------ | ------- | ----------- | --- | ------ | -------- | ---------- |
|     |     |     |     |     |     | of 128 | 128 and | a cell-size | of  | 32 32, | as shown | in Table5. |
1to50.
Table6showedanimprovedclassificationaccuracyof94.07%for
TheNCM-DOA-SVMmodelwastrainedonadatasetof1,512
imagesofthesamedimensionsandcell-sizewhenthenumberof
| CLFP images, | and | its performance | was | assessed using | standard |     |     |     |     |     |     |     |
| ------------ | --- | --------------- | --- | -------------- | -------- | --- | --- | --- | --- | --- | --- | --- |
dolphinsmatchedthenumberofrepetitions.Additionally,Tables7,
evaluationmetrics.Theresultsareasgivenbelow:
8reportedafurtherincreaseinclassificationaccuracyto96.03and
98.0%underthesameconditions.Thesefindingsarecorroborated
A =95.1±1.2%, R =94.8±1.4%, S =95.3±1.1% by the data in Table9. The DOA showed an average accuracy
| CV  |     | CV  |     | CV  |     |     |     |     |     |     |     |     |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
BootstrapCI=[93.9%,96.3%], Convergence=23iterations rateof91.0%,whereastheDOA-SVMhybridalgorithmachieved
|     |     |     |     |     |     | an average | accuracy | of 94.07%, | the | FCM-DOA-SVM |     | algorithm |
| --- | --- | --- | --- | --- | --- | ---------- | -------- | ---------- | --- | ----------- | --- | --------- |
Inordertoanalyzeandidentifythe504imagesinthedataset attained an average accuracy of 96.03% and NCM-DOA-SVM
asCLFP,theNCM-DOA-SVMsystemhasbeenused.Table8lists algorithmreachedanaverageaccuracyof98.0%.
the training phase results and provides an explanation of the Our experimental findings indicate that the hybrid algorithm
testing phase results using the suggested hybrid approach. The DOA-SVM demonstrates a higher classification ratio than the
following parameter settings were used when implementing the DOA. This improvement is attributed to the DOA optimization
| FrontiersinBigData |     |     |     |     |     | 13  |     |     |     |     |     | frontiersin.org |
| ------------------ | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --------------- |

RachelandDevarasan 10.3389/fdata.2025.1641714
FIGURE4
NeutrosophiclogicT/I/Fmembershipvisualization.
capabilities, which emulate the cooperative behavior of dolphins Thisapproachisconvenientforhandlingunclear,overlappingand
to enhance parameter selection. The algorithm improves uncertain segments within the data and this quality is key for
convergence speed and reduces computational cost by learning classifyingcomplicateddatasets.Ontheotherhand,theDOAhelps
from previous configurations. A well-tuned DOA balances withglobaloptimization,sofeaturescanbeeffectivelychosenand
exploration and exploitation, making parameter selection crucial parameters can be successfully tuned. Finally, the SVM provides
foroptimalperformance. goodperformanceinthefinalclassificationstage.Theintegration
According to experimental results, the suggested hybrid of all three methods together creates a better and more efficient
algorithm,whichcombinesDOA-SVMwithFCM,performsbetter resultsthanifonlytwoareused,suchasDOAorDOA-SVM.
intermsofclassificationaccuracythanboththestandaloneDOA In this neutrosophic logic visualization (see Figure4), the
andthehybridDOA-SVMalgorithm.Theefficientcombinationof T/I/F membership framework provides significant advantages
FCMandDOA,whereeachtechniqueimprovestheperformanceof for our database. Specifically, Truth membership (T) highlights
theother,isthesourceofthisimprovement.Whenthesealgorithms robust and well-defined clusters at the core of the dataset, while
are used together, they perform better on classification tasks and Indeterminacymembership(I)capturestransitionalandboundary
othertasksthanwhentheyareusedseparately.TheFCMalgorithm regionsâA˘Tˇeffectively representing ambiguous cases that are
helpsbyprovidingsoftaggregationcapabilities,whichenablemore often misclassified by traditional methods. Falsity membership
precisedatapointclassification,particularlywhenfeaturesoverlap. (F) excels at identifying outliers, enhancing data integrity by
The DOA also supports more efficient searching, so that key revealingnon-conformingpatternsthatmaycompromiseanalysis.
data points can be identified and the required computations can The integrated T/I/F analysis enables a comprehensive and
bereduced. nuanced characterization of uncertainty, offering deeper insights
Within the spectrum of assessed approaches, the NCM- and improved decision-making accuracy over conventional
DOA-SVMmethodconsistentlymaintainedsuperiorclassification single-valueapproaches,thusestablishingneutrosophiclogicasa
accuracy. This enhanced performance is resulted due to the superiortoolforextractingactionableinformationfromuncertain
effectivecombinationofNCMclustering,theDOA,andtheSVM. databases.
FrontiersinBigData 14 frontiersin.org

| RachelandDevarasan |     |     |     |     |     |     |     |     |     |     |     | 10.3389/fdata.2025.1641714 |     |     |     |
| ------------------ | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | -------------------------- | --- | --- | --- |
TABLE10 Performancecomparisonofbio-inspiredoptimization framework that successfully merges the Dolphin Optimization
algorithmswithSVM.
|           |     |          |     |           |     |            |     | Algorithm      | with a Support | Vector            | Machine        |     | classifier,   | establishing |         |
| --------- | --- | -------- | --- | --------- | --- | ---------- | --- | -------------- | -------------- | ----------------- | -------------- | --- | ------------- | ------------ | ------- |
|           |     |          |     |           |     |            |     | a new approach | for            | CLFP recognition. |                | The | research      | is           | further |
| Algorithm |     | Mean     |     | Standard  |     | Mean       |     |                |                |                   |                |     |               |              |         |
|           |     | accuracy |     | deviation |     | processing |     |                |                |                   |                |     |               |              |         |
|           |     |          |     |           |     |            |     | advanced       | by integrating | fuzzy             | clustering     |     | to            | refine       | feature |
|           |     | (%)      |     |           |     | time(s)    |     |                |                |                   |                |     |               |              |         |
|           |     |          |     |           |     |            |     | representation | and,           | most              | significantly, |     | by pioneering |              | the     |
±0.0171 application of Neutrosophic C-Means clustering to effectively
| BaselineSVM |     | 88.33 |     |     |     | 3.52 |     |     |     |     |     |     |     |     |     |
| ----------- | --- | ----- | --- | --- | --- | ---- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
±0.0132 managetheinherentuncertaintyandindeterminacyinfingerprint
| DOA-SVM |     | 89.69 |     |         |     | 2073.12  |     |                |            |               |     |          |          |           |     |
| ------- | --- | ----- | --- | ------- | --- | -------- | --- | -------------- | ---------- | ------------- | --- | -------- | -------- | --------- | --- |
|         |     |       |     |         |     |          |     | images. Beyond | accuracy   | improvements, |     | a        | thorough | analysis  | of  |
| PSO-SVM |     | 89.65 |     | ±0.0136 |     | 3,722.63 |     |                |            |               |     |          |          |           |     |
|         |     |       |     |         |     |          |     | computational  | efficiency | is provided,  |     | offering | valuable | practical |     |
GA-SVM 89.26 ±0.0111 1,547.74 insights that bridge the gap between algorithmic innovation and
real-worldsystemdeployment.
| ACO-SVM |     | 89.45 |     | ±0.0142 |     | 3,304.97 |     |     |     |     |     |     |     |     |     |
| ------- | --- | ----- | --- | ------- | --- | -------- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
Despitethepromisingresults,thisstudyhascertainlimitations.
Boldvaluesindicatethebest(highest-performing)resultsamongthecomparedmethods.
Thegeneralizabilityofourfindingsisconstrainedbythedataset’s
In this work, we propose a CLFP recognition system based limited size and demographic diversity, which may not fully
on the DOA with 3,902.533 processing seconds and 91.0% represent broader populations. Furthermore, the robustness of
|     |     |     |     |     |     |     |     | the single-stage | classification |     | pipeline | requires | further | validation |     |
| --- | --- | --- | --- | --- | --- | --- | --- | ---------------- | -------------- | --- | -------- | -------- | ------- | ---------- | --- |
classificationaccuracy.Additionally,wecombinetheDOAandthe
SVMalgorithmstoproposeanewhybridalgorithm.Bysuggesting against low-quality, noisy, or incomplete fingerprint images. To
anewfitnessfunctionbasedontheSVMalgorithmthatdepends addresstheseconstraintsandadvancethisresearch,futureefforts
ondolphinvaluesratherthantheoriginalparameters(G,(cid:7),A),the
|     |     |     |     |     |     |     |     | should prioritize | several | key | areas. | Expanding |     | the evaluation |     |
| --- | --- | --- | --- | --- | --- | --- | --- | ----------------- | ------- | --- | ------ | --------- | --- | -------------- | --- |
combinationaimstoimproveclassificationaccuracy.Thismethod to include larger, multi-ethnic datasets is crucial for verifying
performanceacrossdiversereal-worldconditions.Exploringmulti-
| took 5,039.768 |     | s to process | and | had a classification |     | accuracy | of  |     |     |     |     |     |     |     |     |
| -------------- | --- | ------------ | --- | -------------------- | --- | -------- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
94.07%.Inordertofurtherincreaseclassificationaccuracy,wealso stage architectures could also enhance robustness by decoupling
suggest another hybrid algorithm that combines the DOA-SVM processes like feature enhancement and classification. Finally,
andtheFCMalgorithm.IncomparisontoboththeDOAandthe developingreal-timeprototypeswouldbeinvaluableforassessing
DOA-SVMalgorithm,thisalgorithmefficientlyhandlesambiguous the practical deployment of these methods, particularly under
computationalconstraints.Theseresearchdirectionsareessential
| and imprecise |     | data, resulting | in  | a more | accurate | identification |     |     |     |     |     |     |     |     |     |
| ------------- | --- | --------------- | --- | ------ | -------- | -------------- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
system and attaining classification accuracy of 96.03% with a for translating the current algorithmic innovations into reliable,
processingtimeof1,684.920s.WesuggestcombiningNCMwith real-worldbiometricsystems.
DOA-SVMinplaceofFCM.Thisresultsinahigherclassification
accuracyof98.0%ataprocessingtimeof1,352.024s.TheNCM-
|     |     |     |     |     |     |     |     | Data | availability | statement |     |     |     |     |     |
| --- | --- | --- | --- | --- | --- | --- | --- | ---- | ------------ | --------- | --- | --- | --- | --- | --- |
DOA-SVMoutperformsothermethodsforCLFPimages.
| The | comparative | evaluation |     | of bio-inspired |     | optimization |     |     |     |     |     |     |     |     |     |
| --- | ----------- | ---------- | --- | --------------- | --- | ------------ | --- | --- | --- | --- | --- | --- | --- | --- | --- |
algorithms (see Table10) reveals that DOA-SVM achieves the Theoriginalcontributionspresentedinthestudyareincluded
|     |     |     |     |     |     |     |     | in the article/supplementary |     |     | material, | further | inquiries |     | can be |
| --- | --- | --- | --- | --- | --- | --- | --- | ---------------------------- | --- | --- | --------- | ------- | --------- | --- | ------ |
highestclassificationaccuracyof89.69%withtheloweststandard
directedtothecorrespondingauthor.
| deviation   | (±0.0132), | demonstrating |                     |     | superior    | performance |       |     |     |     |     |     |     |     |     |
| ----------- | ---------- | ------------- | ------------------- | --- | ----------- | ----------- | ----- | --- | --- | --- | --- | --- | --- | --- | --- |
| consistency | compared   | to            | other metaheuristic |     | approaches. |             | While |     |     |     |     |     |     |     |     |
all bio-inspired methods substantially outperform the baseline Ethics statement
| SVM (88.33%), |          | the computational |               | cost varies | significantly, |            | with |             |           |        |      |          |     |             |     |
| ------------- | -------- | ----------------- | ------------- | ----------- | -------------- | ---------- | ---- | ----------- | --------- | ------ | ---- | -------- | --- | ----------- | --- |
| GA-SVM        | offering | the best          | time-accuracy |             | trade-off      | (89.26%    | in   |             |           |        |      |          |     |             |     |
|               |          |                   |               |             |                |            |      | The studies | involving | humans | were | approved |     | by Hongkong |     |
| 1,547.74      | ss) and  | PSO-SVM           | requiring     |             | the highest    | processing |      |             |           |        |      |          |     |             |     |
PolytechnicUniversity.Thestudieswereconductedinaccordance
time (3,722.63 s) for comparable accuracy. The results validate with the local legislation and institutional requirements. The
| DOA’s effectiveness |     | in hyperparameter |     |     | optimization |     | for CLFP |     |     |     |     |     |     |     |     |
| ------------------- | --- | ----------------- | --- | --- | ------------ | --- | -------- | --- | --- | --- | --- | --- | --- | --- | --- |
participantsprovidedtheirwritteninformedconsenttoparticipate
| recognition, | though | the | 588-fold | increase | in  | processing | time |     |     |     |     |     |     |     |     |
| ------------ | ------ | --- | -------- | -------- | --- | ---------- | ---- | --- | --- | --- | --- | --- | --- | --- | --- |
inthisstudy.
comparedtobaselineSVMhighlightsthecomputationaloverhead
inherentinpopulation-basedoptimizationapproaches.
|              |     |     |        |     |      |     |     | Author          | contributions |           |           |         |              |          |         |
| ------------ | --- | --- | ------ | --- | ---- | --- | --- | --------------- | ------------- | --------- | --------- | ------- | ------------ | -------- | ------- |
| 5 Conclusion |     | and | future |     | work |     |     |                 |               |           |           |         |              |          |         |
|              |     |     |        |     |      |     |     | JR: Data        | curation,     | Formal    | analysis, |         | Methodology, |          | Project |
|              |     |     |        |     |      |     |     | administration, | Resources,    | Software, |           | Writing | –            | original | draft.  |
This study demonstrates that strategic hybridization of bio- ED: Conceptualization, Funding acquisition, Investigation,
inspiredoptimizationwithmachinelearningclassifierssignificantly
|          |             |             |     |              |     |                |     | Supervision, | Validation, | Visualization, |     | Writing |     | – review | &   |
| -------- | ----------- | ----------- | --- | ------------ | --- | -------------- | --- | ------------ | ----------- | -------------- | --- | ------- | --- | -------- | --- |
| advances | contactless | fingerprint |     | recognition. |     | Our systematic |     |              |             |                |     |         |     |          |     |
editing.
evaluationrevealsclearperformanceprogression:DOA(91.00%),
DOA-SVM(94.07%),FCM-DOA-SVM(96.03%),andNCM-DOA-
Funding
| SVM (98.00%), |     | with the | NCM-based | hybrid | achieving |     | superior |     |     |     |     |     |     |     |     |
| ------------- | --- | -------- | --------- | ------ | --------- | --- | -------- | --- | --- | --- | --- | --- | --- | --- | --- |
accuracywhilemaintainingcompetitiveprocessingefficiency.
This work makes several key contributions to the field of The author(s) declare that financial support was
contactless biometrics. Primarily, it introduces a novel hybrid received for the research and/or publication of this article.
| FrontiersinBigData |     |     |     |     |     |     |     | 15  |     |     |     |     |     | frontiersin.org |     |
| ------------------ | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --------------- | --- |

| RachelandDevarasan |         |          |     |            |           |     |                 |      |      |                |     | 10.3389/fdata.2025.1641714 |         |         |
| ------------------ | ------- | -------- | --- | ---------- | --------- | --- | --------------- | ---- | ---- | -------------- | --- | -------------------------- | ------- | ------- |
| Open access        | funding | provided |     | by Vellore | Institute |     | of              |      |      |                |     |                            |         |         |
|                    |         |          |     |            |           |     | Any alternative | text | (alt | text) provided |     | alongside                  | figures | in this |
Technology. articlehasbeengeneratedbyFrontierswiththesupportofartificial
|     |     |     |     |     |     |     | intelligence | and | reasonable | efforts | have | been | made to | ensure |
| --- | --- | --- | --- | --- | --- | --- | ------------ | --- | ---------- | ------- | ---- | ---- | ------- | ------ |
accuracy,includingreviewbytheauthorswhereverpossible.Ifyou
| Conflict | of interest |     |     |     |     |     |     |     |     |     |     |     |     |     |
| -------- | ----------- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
identifyanyissues,pleasecontactus.
| The authors | declare | that | the research | was | conducted | in  | the         |     |      |     |     |     |     |     |
| ----------- | ------- | ---- | ------------ | --- | --------- | --- | ----------- | --- | ---- | --- | --- | --- | --- | --- |
|             |         |      |              |     |           |     | Publisher’s |     | note |     |     |     |     |     |
absenceofanycommercialorfinancialrelationshipsthatcouldbe
construedasapotentialconflictofinterest.
|            |     |           |     |     |     |     | All claims     | expressed   |             | in this  | article      | are solely | those            | of the     |
| ---------- | --- | --------- | --- | --- | --- | --- | -------------- | ----------- | ----------- | -------- | ------------ | ---------- | ---------------- | ---------- |
|            |     |           |     |     |     |     | authors and    | do not      | necessarily |          | represent    | those      | of their         | affiliated |
| Generative | AI  | statement |     |     |     |     |                |             |             |          |              |            |                  |            |
|            |     |           |     |     |     |     | organizations, | or          | those       | of the   | publisher,   | the        | editors          | and the    |
|            |     |           |     |     |     |     | reviewers.     | Any product |             | that may | be evaluated |            | in this article, | or         |
Theauthor(s)declarethatnoGenAIwasusedinthecreation claimthatmaybemadebyitsmanufacturer,isnotguaranteedor
| ofthismanuscript. |     |     |     |     |     |     | endorsedbythepublisher. |     |     |     |     |     |     |     |
| ----------------- | --- | --- | --- | --- | --- | --- | ----------------------- | --- | --- | --- | --- | --- | --- | --- |
References
Al-Taie, S. A. M., and Khaleel, B. I. (2024). Palmprint identification using Grosz, S. A., Engelsma, J. J., Liu, E., and Jain, A. K. (2021). C2CL: contact
dolphin optimization. Period. Polytech. Electr. Eng. Comput. Sci. 68, 295–308. tocontactlessfingerprintmatching.IEEETrans.Inf.ForensicsSecur.17,196–210.
| doi:10.3311/PPee.22767 |     |     |     |     |     |     | doi:10.1109/TIFS.2021.3134867 |     |     |     |     |     |     |     |
| ---------------------- | --- | --- | --- | --- | --- | --- | ----------------------------- | --- | --- | --- | --- | --- | --- | --- |
Artan,Y.(2024).“MinNetbasedcontactlessfingerprintmatchingmethod,”in2024 Guo,Y.,andSengur,A.(2015).NCM:neutrosophicc-meansclusteringalgorithm.
32ndSignalProcessingandCommunicationsApplicationsConference(SIU)(Mersin),
PatternRecognit.48,2710–2724.doi:10.1016/j.patcog.2015.02.018
1-4.doi:10.1109/SIU61531.2024.10600842
|              |         |            |         |            |     |         | Huang, C. | L., and | Wang, | C. J. (2006). | A   | GA-based | feature selection | and |
| ------------ | ------- | ---------- | ------- | ---------- | --- | ------- | --------- | ------- | ----- | ------------- | --- | -------- | ----------------- | --- |
| Attrish, A., | Bharat, | N., Anand, | V., and | Kanhangad, | V.  | (2021). | A         |         |       |               |     |          |                   |     |
parametersoptimizationforsupportvectormachines.ExpertSyst.Appl.31,231–240.
contactless fingerprint recognition system. arXiv [preprint]. arXiv:2108.09048. doi:10.1016/j.eswa.2005.09.024
doi:10.48550/arXiv.2108.09048
|     |     |     |     |     |     |     | Jawade, B., | Mohan, | D. D., | Setlur, S., | Ratha, | N., and Govindaraju, |     | V. (2022). |
| --- | --- | --- | --- | --- | --- | --- | ----------- | ------ | ------ | ----------- | ------ | -------------------- | --- | ---------- |
Bezdek, J. C., Ehrlich, R., and Full, W. (1984). FCM: the fuzzy c-means “RidgeBase: a cross-sensor multi-finger contactless fingerprint dataset," in 2022
clustering algorithm. Comput. Geosci. 10, 191–203. doi: 10.1016/0098-3004(84) IEEEInternationalJointConferenceonBiometrics(IJCB)(AbuDhabi:IEEE),1–9.
90020-7
doi:10.1109/IJCB54206.2022.10007936
Cetina,K.,Márquez-Neila,P.,andBaumela,L.(2014).“Acomparativestudyof Kaplesh,P.,Gupta,A.,Bansal,D.,Sofat,S.,andMittal,A.(2024).Visiontransformer
featuredescriptorsformitochondriaandsynapsesegmentation,”inProceedingsof
|     |     |     |     |     |     |     | for contactless | fingerprint | classification. |     | Multimedia | Tools Appl. | 84, 31239–31259. |     |
| --- | --- | --- | --- | --- | --- | --- | --------------- | ----------- | --------------- | --- | ---------- | ----------- | ---------------- | --- |
the22ndInternationalConferenceonPatternRecognition(Stockholm),3215-3220. doi:10.1007/s11042-024-20396-4
doi:10.1109/ICPR.2014.554
|     |     |     |     |     |     |     | Kaveh, A., | and Farhoudi, |     | N. (2013). | A new | optimization | method: | Dolphin |
| --- | --- | --- | --- | --- | --- | --- | ---------- | ------------- | --- | ---------- | ----- | ------------ | ------- | ------- |
Chicco,D.,andJurman,G.(2020).TheadvantagesoftheMatthewscorrelation echolocation. Adv. Eng. Softw. 59, 53–70. doi: 10.1016/j.advengsoft.2013.
coefficient(MCC)overF1scoreandaccuracyinbinaryclassificationevaluation.BMC
03.004
Genomics21:6.doi:10.1186/s12864-019-6413-7
Chimatapu,R.,Hagras,H.,Starkey,A.,andOwusu,G.(2018).“ExplainableAIand Kaveh, A., Hoseini Vaez, S. R., and Hosseini, P. (2017). Modified dolphin
monitoringoperatorforweightoptimizationofframestructures.Period.Polytech.Civ.
fuzzylogicsystems,"inTheoryandPracticeofNaturalComputing(Cham:Springer),
Eng.61,770–779.doi:10.3311/PPci.9691
3–20.doi:10.1007/978-3-030-04070-3_1
Labati,R.D.,Genovese,A.,Piuri,V.,andScotti,F.(2014).Touchlessfingerprint
Chowdhury,A.M.,andImtiaz,M.H.(2022).Contactlessfingerprintrecognition
biometrics:asurveyon2Dand3Dtechnologies.J.InternetTechnol.15,325–332.
using deep learning—a systematic review. J. Cybersecur. Priv. 2, 714–730. doi:10.6138/JIT.2014.15.3.01
doi:10.3390/jcp2030036
Lin,C.,andKumar,A.(2018).Matchingcontactlessandcontact-basedconventional
Cortes, C., and Vapnik, V. (1995). Support-vector networks. Mach. Learn. 20, fingerprint images for biometrics identification. IEEE Trans. Image Process. 27,
273–297.doi:10.1023/A:1022627411411
2008–2021.doi:10.1109/TIP.2017.2788866
| Cui, Z., | Jia, Y., Zheng, | S., | and Su, F. | (2024). | Contactless | fingerprint |     |     |     |     |     |     |     |     |
| -------- | --------------- | --- | ---------- | ------- | ----------- | ----------- | --- | --- | --- | --- | --- | --- | --- | --- |
Lin,S.W.,Ying,K.C.,Chen,S.C.,andLee,Z.J.(2008).Particleswarmoptimization
| recognition using | 3D  | graph matching. | arXiv | [preprint]. | arXiv:2409.08782. |     |     |     |     |     |     |     |     |     |
| ----------------- | --- | --------------- | ----- | ----------- | ----------------- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
forparameterdeterminationandfeatureselectionofsupportvectormachines.Expert
doi:10.48550/arXiv.2409.08782 Syst.Appl.35,1817–1824.doi:10.1016/j.eswa.2007.08.088
Dong,C.,andKumar,A.(2023).Synthesisofmulti-view3Dfingerprintstoadvance
|     |     |     |     |     |     |     | Maltoni, | D., Maio, | D., Jain, | A. K., | and Prabhakar, |     | S. (2009). | Handbook |
| --- | --- | --- | --- | --- | --- | --- | -------- | --------- | --------- | ------ | -------------- | --- | ---------- | -------- |
contactless fingerprint identification. IEEE Trans. Pattern Anal. Mach. Intell. 45, of Fingerprint Recognition, Vol. 2. Cham: Springer. doi: 10.1007/978-1-8488
13134–13151.doi:10.1109/TPAMI.2023.3294357
2-254-2
Elhariri,E.,El-Bendary,N.,Hassanien,A.E.,andSnasel,V.(2015).Anassistive Mendel,J.M.,andBonissone,P.P.(2021).Criticalthinkingaboutexplainable
objectrecognitionsystemforenhancingseniorsqualityoflife.ProcediaComput.Sci.
|     |     |     |     |     |     |     | AI (XAI) for | rule-based | fuzzy | systems. | IEEE Trans. | Fuzzy | Syst. 29, 3579–3593. |     |
| --- | --- | --- | --- | --- | --- | --- | ------------ | ---------- | ----- | -------- | ----------- | ----- | -------------------- | --- |
65,691–700.doi:10.1016/j.procs.2015.09.013
doi:10.1109/TFUZZ.2021.3079503
Faye,A.,Ndaw,J.D.,andSène,M.(2018).“SVM-basedDOAestimationwith
Moen,U.(2018).EarlyDetectionofAlzheimer’sDiseaseusing3DTextureFeatures
| classification | optimization," | in 2018 | 26th Telecommunications |     | Forum | (TELFOR) |     |     |     |     |     |     |     |     |
| -------------- | -------------- | ------- | ----------------------- | --- | ----- | -------- | --- | --- | --- | --- | --- | --- | --- | --- |
(Belgrade:IEEE),1–4.doi:10.1109/TELFOR.2018.8611827 and3DConvolutionalNeuralNetworksfromstructuralMRI[MScThesis].University
ofStavanger.Availableonlineat:http://hdl.handle.net/11250/2564781(AccessedJuly
| Ferreyra,E.,Hagras,H.,Kern,M.,andOwusu,G.(2019).“Depictingdecision- |       |             |             |            |              |        | 28,2025).            |     |     |        |                   |     |                     |     |
| ------------------------------------------------------------------- | ----- | ----------- | ----------- | ---------- | ------------ | ------ | -------------------- | --- | --- | ------ | ----------------- | --- | ------------------- | --- |
| making: a type-2                                                    | fuzzy | logic based | explainable | artificial | intelligence | system |                      |     |     |        |                   |     |                     |     |
|                                                                     |       |             |             |            |              |        | Mohamed-Abdul-Cader, |     | A., | Banks, | J., and Chandran, | V.  | (2023). Fingerprint |     |
for goal-driven simulation in the workforce allocation domain," in 2019 IEEE systems:sensors,imageacquisition,interoperabilityandchallenges.Sensors23:6591.
| InternationalConferenceonFuzzySystems(FUZZ-IEEE)(NewOrleans,LA:IEEE), |     |     |     |     |     |     | doi:10.3390/s23146591 |     |     |     |     |     |     |     |
| --------------------------------------------------------------------- | --- | --- | --- | --- | --- | --- | --------------------- | --- | --- | --- | --- | --- | --- | --- |
1–6.doi:10.1109/FUZZ-IEEE.2019.8858933
|     |     |     |     |     |     |     | Peddi, S., | Bandyopadhyay, |     | S., and | Samanta, | D. (2025). | G-MSGINet: | a   |
| --- | --- | --- | --- | --- | --- | --- | ---------- | -------------- | --- | ------- | -------- | ---------- | ---------- | --- |
Gholizadeh,S.,andPoorhoseini,H.(2016).Seismiclayoutoptimizationofsteel grouped multi-scale graph-involution network for contactless fingerprint
bracedframesbyanimproveddolphinecholocationalgorithm.Struct.Multidiscip. recognition. arXiv [preprint]. arXiv:2505.08233. doi: 10.48550/arXiv.2505.
| Optim.54,1011–1029.doi:10.1007/s00158-016-1461-y |     |     |     |     |     |     | 08233 |     |     |     |     |     |                 |     |
| ------------------------------------------------ | --- | --- | --- | --- | --- | --- | ----- | --- | --- | --- | --- | --- | --------------- | --- |
| FrontiersinBigData                               |     |     |     |     |     |     | 16    |     |     |     |     |     | frontiersin.org |     |

RachelandDevarasan 10.3389/fdata.2025.1641714
Priesnitz,J.,Rathgeb,C.,Buchmann,N.,Busch,C.,andMargraf,M.(2021).An International Conference on Machine Learning and Computing, Bangalore, India
overviewoftouchless2Dfingerprintrecognition.EURASIPJImageVideoProcess2021, (Bangalore:IEEE),101–105.doi:10.1109/ICMLC.2010.72
1–21.doi:10.1186/s13640-021-00548-4
Soto, R.,Crawford, B.,Carrasco, C., Almonacid, B.,Reyes, V.,Araya, I., etal.
Qi,Z.,Tian,Y.,andShi,Y.(2013).Robusttwinsupportvectormachineforpattern (2016).“Solvingmanufacturingcelldesignproblemsbyusingadolphinecholocation
classification.PatternRecognit.46,305–316.doi:10.1016/j.patcog.2012.06.019 algorithm," in Computational Science and Its Applications – ICCSA 2016 (Cham:
Springer),77–86.doi:10.1007/978-3-319-42092-9_7
Qiao,W.,andYang,Z.(2019).Modifieddolphinswarmalgorithmbasedonchaotic
mapsforsolvinghigh-dimensionalfunctionoptimizationproblems.IEEEAccess7, Sreehari, S., and Anzar, M. (2025). Touchless fingerprint recognition: a
110472–110486.doi:10.1109/ACCESS.2019.2931910 survey of recent developments and challenges. Comput. Electr. Eng. 122:109894.
doi:10.1016/j.compeleceng.2024.109894
Rajaram,K.,Amma,N.,andSelvakumar,S.(2023).Convolutionalneuralnetwork
basedchildrenrecognitionsystemusingcontactlessfingerprints.Int.J.Inf.Technol.15, Tan, H., and Kumar, A. (2020). Towards more accurate contactless fingerprint
2695–2705.doi:10.1007/s41870-023-01306-7 minutiaeextractionandpose-invariantmatching.IEEETrans.Inf.ForensicsSecur.15,
3924–3937.doi:10.1109/TIFS.2020.3001732
Ruzicka,L.,Kohn,B.,andHeitzinger,C.(2025).TipSegNet:fingertipsegmentation
incontactlessfingerprintimaging.Sensors25:1824.doi:10.3390/s25061824 Vibert,B.,LeBars,J.M.,Charrier,C.,andRosenberger,C.(2023).Comparative
studyofminutiaeselectionmethodsfordigitalfingerprints.Front.BigData6:1146034.
Shao,Y.H.,Zhang,C.H.,Wang,X.B.,andDeng,N.Y.(2011).Improvements
doi:10.3389/fdata.2023.1146034
on twin support vector machines. IEEE Trans. Neural Netw. 22, 962–968.
doi:10.1109/TNN.2011.2130540 Vithlani, P., and Kumbharana, C. K. (2015). Structural and statistical feature
extractionmethodsforcharacteranddigitrecognition.Int.J.Comput.Appl.120,
Sharma,S.,andKaul,A.(2018).Hybridfuzzymulti-criteriadecisionmakingbased
43–47.doi:10.5120/21413-4451
multi cluster head dolphinswarm optimized IDSfor VANET. Veh. Commun. 12,
23–38.doi:10.1016/j.vehcom.2017.12.003 Wu,T.,Yao,M.,andYang,J.(2017).Dolphinswarmextremelearningmachine.
Cognit.Comput.9,275–284.doi:10.1007/s12559-017-9451-y
Shi, L., Lan, S., Gui, H., Yang, Y., and Guo, Z. (2022). A novel 2D
contactless fingerprint matching method. Neurocomputing 500, 547–555. Wu,T.Q.,Yao,M.,andYang,J.H.(2016).Dolphinswarmalgorithm.Front.Inf.
doi:10.1016/j.neucom.2022.05.092 Technol.Electr.Eng.17,717–729.doi:10.1631/FITEE.1500287
Shukla, A. K., Smits, G., Pivert, O., and Lesot, M. J. (2020). “Explaining data Yin,X.,Zhu,Y.,andHu,J.(2020).Contactlessfingerprintrecognitionbasedon
regularitiesandanomalies,"inProc.2020IEEEInternationalConferenceonFuzzy globalminutiatopologyandloosegeneticalgorithm.IEEETrans.Inf.ForensicsSecur.
Systems(FUZZ-IEEE)(Glasgow:IEEE),1–8.doi:10.1109/FUZZ48607.2020.9177689 15,28–41.doi:10.1109/TIFS.2019.2918083
Siddiqui,M.,andIqbal,S.AlShammari,B.,Alhaqbani,B.,Khan,T.,Razzak,I. Yin,X.,Zhu,Y.,andHu,J.(2021).Asurveyon2Dand3Dcontactlessfingerprint
(2024).“Arobustalgorithmforcontactlessfingerprintenhancementandmatching," biometrics:ataxonomy,review,andfuturedirections.IEEEOpenJ.Comput.Soc.2,
in 2024 International Conference on Digital Image Computing: Techniques and 370–381.doi:10.1109/OJCS.2021.3119572
Applications(DICTA)(Perth:IEEE),214–220.doi:10.1109/DICTA63115.2024.00041
Yong,W.,Tao,W.,Cheng-Zhi,Z.,andHua-Juan,H.(2016).Anewstochastic
Smarandache, F. (2003). A Unifying Field in Logics: Neutrosophic Logic. optimizationapproach—dolphinswarmoptimizationalgorithm.Int.J.Comput.Intell.
Neutrosophy, Neutrosophic Set, Neutrosophic Probability and Statistics.Austin, TX: Appl.15:1650011.doi:10.1142/S1469026816500115
AmericanResearchPress.
Zhang,Z.,Liu,S.,andLiu,M.(2021).Amulti-taskfullydeepconvolutionalneural
Soranamageswari, M., and Meena, C. (2010). “Statistical feature extraction for networkforcontactlessfingerprintminutiaeextraction.PatternRecognit.120:108189.
classificationofimagespamusingartificialneuralnetworks,”inProc.2010Second doi:10.1016/j.patcog.2021.108189
FrontiersinBigData 17 frontiersin.org