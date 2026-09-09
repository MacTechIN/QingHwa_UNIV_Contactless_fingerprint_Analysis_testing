Since January 2020 Elsevier has created a COVID-19 resource centre with

free information in English and Mandarin on the novel coronavirus COVID-

19. The COVID-19 resource centre is hosted on Elsevier Connect, the

company's public news and information website.

Elsevier hereby grants permission to make all its COVID-19-related

research that is available on the COVID-19 resource centre - including this

research content - immediately available in PubMed Central and other

publicly funded repositories, such as the WHO COVID database with rights

for unrestricted research re-use and analyses in any form or by any means

with acknowledgement of the original source. These permissions are

granted for free by Elsevier for as long as the COVID-19 resource centre

remains active.

Contents lists available at ScienceDirect

Array

journal homepage: www.sciencedirect.com/journal/array

Towards a high-precision contactless fingerprint scanner for
biometric authentication

Uzoma I. Oduah a, *, Ifeanyichukwu F. Kevin b, Daniel O. Oluwole b, Josephat U. Izunobi c
a Department of Physics, University of Lagos, Akoka-Yaba, 100213, Lagos, Nigeria
b Department of Electrical and Electronics Engineering, University of Lagos, Akoka-Yaba, 100213, Lagos, Nigeria
c Department of Chemistry, University of Lagos, Akoka-Yaba, 100213, Lagos, Nigeria

A R T I C L E  I N F O

A B S T R A C T

Keywords:
Authentication
Biometric
Blue light
COVID-19
Fingerprint
Pattern recognition

The  raging  COVID-19  pandemic  accentuates  the  urgent  and  compelling  need  for  non-contact  fingerprinting
biometric  authentication  devices  to  mitigate  the  transmission  of  the  severe  acute  respiratory  syndrome  coro-
navirus  2  (SARS-CoV-2)  and  other  contagious  infections.  Current  approaches  to  contactless  fingerprinting
scanners  suffer  limitations  ranging  from  poor  compatibility  with  two-dimensional  equivalent  touch-based
fingerprint  images  to  perspective  distortions,  inconstant  resolution,  motion  blur  images  and  low  correlation
factors. Herein, these constraints are tackled by implementing a system that enables the positioning of the target
finger(s) at fixed vertical and horizontal distances away from the camera lens without the physical contact of the
fingers  with  the  device  framework  during scanning.  A high-precision  fingerprint pattern  recognition  of  up  to
97.51%  correlation  factor  has  been  achieved,  using  this  contactless  method,  by  varying  the  background  illu-
minating light and implementing two-dimensional imaging techniques and near-constant resolution. Addition-
ally, a convenient contactless fingerprint acquisition process is reinforced through a unique architectural design.

Credit author statement

Conceptualization: Uzoma I. Oduah, Investigation: Ifeanyichukwu F.
Kevin  and  Daniel  O.  Oluwole,  Methodology:  Uzoma  I.  Oduah,  Ifea-
nyichukwu F. Kevin and, Daniel O. Oluwole: Analysis: Uzoma I. Oduah
and  Josephat  U.  Izunobi,  Writing  –  Original  Draft,  Uzoma  I.  Oduah,
Ifeanyichukwu  F.  Kevin,  Daniel  O.  Oluwole  and  Josephat  U.  Izunobi,
Writing – Review & Editing, Josephat U. Izunobi and Uzoma I. Oduah.

1. Introduction

There is an urgent and growing need, in today’s rapidly changing and
disease-prone world, for non-contact biometric authentication devices
to  aid  hygiene  and  safety,  and  mitigate  the  spread  of  contagious  in-
fections [1]. Fingerprint biometrics is one of the most widely used forms
of recognition for the purposes of identification and authentication due
to its convenience, reliability and accuracy but it is also one of the most
susceptible agents of transmission for contagious infections [43]. This
drawback  is  especially  accentuated,  now,  in  the  wake  of  the  recent
emergence  of  the  severe  acute  respiratory  syndrome  coronavirus  2
(SARS-CoV-2) [3].

Prior to the onset of the COVID-19 pandemic, however, efforts had
been on-going towards the development of contactless fingerprint bio-
metric  scanners  mainly  because  of  the  limitations  of  touch-based
fingerprint scanners. Some of the limitations of these scanners, which
operate on optical or capacitive systems, include: non-linear distortions;
introduced by the elastic deformation of the skin during scans, irregular
attributes associated with dry skin, diseases, sweat, dirt and humidity,
security issues; with latent fingerprints deposited on the scan plate after
each use, which can be fraudulently replicated, and inconsistent pres-
sures of the fingers during scans, amongst others [4–8].

It  is  noteworthy  that  contactless  fingerprint  scanners  have  been
previously developed [9–11] but have suffered some underlying chal-
lenges, such as: inefficient fingerprint acquisition techniques, ineffective
approaches to reconciling differences between the salient features of the
images captured by touch-based fingerprint devices and those captured
via contactless fingerprint devices, and the adaptability of the contact-
less fingerprint system to the automated fingerprint identification sys-
tem  (AFIS);  commonly  used  in  the  touch-based  (contact)  method  of
fingerprinting [9–13].

Two  approaches  have  been  reported  for  contactless  fingerprint
acquisition, namely: two-dimensional (2-D) and three-dimensional (3-

* Corresponding author.

E-mail address: uoduah@unilag.edu.ng (U.I. Oduah).

https://doi.org/10.1016/j.array.2021.100083
Received 11 February 2021; Received in revised form 22 June 2021; Accepted 28 July 2021

Array11(2021)100083Availableonline6August20212590-0056/©2021TheAuthors.PublishedbyElsevierInc.ThisisanopenaccessarticleundertheCCBY-NC-NDlicense(http://creativecommons.org/licenses/by-nc-nd/4.0/).U.I. Oduah et al.

D) imaging techniques. The 2-D method uses a single camera to capture
images  whereas  the  3-D  technique  employs  multiple  cameras,  which
require the superimposition of images and use of complex algorithms
[14–16].  The available contactless fingerprint  2-D scanners are, how-
ever,  limited  by  their  fingerprint  acquisition  styles.  They  are  also
affected by background illumination variations and reflections thereby
producing  poor  resolution  fingerprint  images  [17,19,44].  Unsurpris-
ingly, most of the existing contactless fingerprint scanners deploy the
3-D technique to capture more detailed fingerprints.

The  3-D  approach  emphasizes  on  gathering  more  details  of  the
fingerprint  by  incorporating  the  dimensions  in  the  x,  y,  z  axis.  The
various  3-D  image  acquisition  techniques  are  multiple-view  setup,
structured lights or photometric stereo methods [45]. Unfortunately, the
additional  details  introduce  errors  of  perspective  distortions  and
inconstant resolution, which reduce the correlation factors. The problem
is evident in the computation of touch-equivalent fingerprints from 3-D
to 2-D images [21–23]. Similarly, the absorption of light by the skin’s
epidermis  during  finger  scans  reduces  the  resolution  of  the  images
produced via the contactless method.

The  acceptable  standard  for  the  matching  and  verification  of
fingerprinting patterns is governed by the automated fingerprint iden-
tification  systems  (AFIS),  which  makes  it  mandatory  for  acquired
fingerprint images, whether 2-D or 3-D, to be converted to the equiva-
lent touch fingerprint 2-D images, described as touch-equivalent images
(TEI),  for  uniformity  and  compatibility  [24–26,46].  It  is  notable  that
available contactless fingerprint scanners use neural network classifiers
to  reconstruct  the  generated  three-dimensional  images  to  equivalent
two-dimensional touch images.

The correlation factor is a key factor in the evaluation of the per-
formance of fingerprint scanners [28]. It is a percentage measure of how
the enrolled fingerprint image matches the one captured during verifi-
cation. It is therefore crucial for the enrolled true fingerprint images,
with  the  extracted  unique  minutiae  features  of  ridges  and  valleys,  to
match  the  captured  fingerprint  images  at  the  point  of  verification
[28–32]. This is termed pattern recognition.

Several  strategies  have  been  implemented  towards  improving  the
correlation  factors  of  contactless  fingerprint  scanners  compared  to
touch-based  fingerprint  scanners,  such  as  the  introduction  of  back-
ground  subtraction  and  contextual  filtering  algorithms  [33]  but  most
contactless fingerprint systems exploit resolution normalization, which
involves  adopting  a  constant  resizing  factor  based  on  the  estimated
distance between the lens camera and target finger, placed for scanning
[34].

It is pertinent to point out that in the touch-based fingerprint scan-
ners, where there is elastic deformation of the finger skin, for instance,
during scans due to pressure, the elastic deformation is compensated for
during  verification  thereby  nullifying  the  error  initially  introduced
during  enrollment  by  the  same  user.  Consequently,  the  fingerprints
captured during verification match the enrolled fingerprints. However,
where enrollment is done using a touch-based fingerprint scanner but
verification is with a contactless fingerprint scanner, the errors due to
elastic deformation become consequential; leading to a mismatch and
poor correlation factor as the captured contactless fingerprints lack the
elastic  deformation  distortions  elicited  by  a  touch-based  fingerprint
scanner.

Furthermore, the scanning of multiple fingers simultaneously is re-
ported  to  enhance  correlation  factor  and  reduce  false  positive  errors
[10].  Generally,  the  method  of  coalescing  multiple  fingers  presents
features,  which  make  sampling  and  pattern  recognition  more  precise
[14].  Nevertheless,  multiple-finger  scanners  require  more  space  and
could be, therefore, considered cumbersome.

A major challenge in the capturing of fingerprints in the contactless
mode remains the positioning of the target finger to maintain a fixed
distance  between  it  and  the  camera  during  scans.  Existing  devices
employ finger placement guides positioned at designated distances from
the lens of the camera; with the fingers oftentimes making contact with

the guides. This was of no concern previously as the focus was to create
higher-resolution  scanners.  With  the  occurrence  of  the  COVID-19
pandemic,  however,  the  need  for  well-resolved,  robust  contactless
fingerprint scanners is more germane and urgent in order to, amongst
others, help in containing the spread of SARS-CoV-2 without compro-
mising on security and safety.

Herein, we outline various efforts implemented towards enhancing
the  correlation  factor  of  the  contactless  fingerprint  scanner  whilst
reducing its false positive errors. In addition, the developed device is
compared with relevant existing technologies to highlight its unique and
innovative  features.  The  major  contributions  of  the  proposed  method
include  achieving  actual  contactless  fingerprint  scans  with  precision,
enhanced  correlation  factors;  derived  from  the  high-precision  finger-
print  acquisition  process,  and  an  effective,  efficient  and  convenient
fingerprinting architectural technique.

2. Materials and method

The  flowchart  for  the  development  of  the  contactless  fingerprint

scanner is presented in Fig. 1.

2.1. Contactless positioning of finger

In  this  work,  a  convenient  and  precise  method  of  positioning  the
finger  in  a  fixed  position,  vertically  and  horizontally,  away  from  the
camera lens, without making contact with the device, was designed and
developed (cf. Fig. 2). With the aid of two infrared-proximity sensors at
the sides of the camera, the target finger was guided until it was properly
aligned at a designated spot in the x, y, z axes. A red-spot light beam
flashed to indicate that the finger was properly aligned in an exact po-
sition  (of  designated  x,  y,  z  coordinates)  and  activate  the  camera  to
capture the image, simultaneously.

A microcontroller was used to perform the logic gate to control and
synchronize the actions of the infrared-proximity sensors with the laser
beam  and  the  camera’s aperture  during  the  scanning  operations.  The
unique architecture of the device provided a non-touch easy guide for
the positioning of the target finger during scanning. Different correla-
tion  factors  were  assessed  based  on  differently  set  vertical  distances
away from the camera in order to identify an ideal distance between the
camera  and  target  finger.  Different  light  intensities  and  wavelengths

Fig. 1. Flowchart for the development of the contactless fingerprint scanner,
showing the component selection and simulation described in the coupling of
the electronic circuit.

Array11(2021)1000832U.I. Oduah et al.

Fig.  2. (a)  Contactless  fingerprint  biometric  scanner  revealing  the  dimensions  of  the  device.  The  opening  guiding  the  placement  of  the  finger  is  4  cm.  A  glass
casement covers the device and protects the electronic components from damage, (b) illustrates the positioning of the target finger at 3.5 cm, away from the camera
during scanning operations. The fingerprint scanner is separated from the area the finger is placed during scanning to avoid physical contact, (c) shows the key
components of the contactless fingerprint scanner, with the location of the camera and two infrared proximity sensors. The infrared proximity sensors and the camera
are in position for the placement of the target finger and (d) shows the opening that guides the alignment of the target finger during the scanning operation.

were also investigated for optimal utility.

2.2. Coupling of the electronic circuit

The  circuit  components  were  coupled  following  the  layout  design
shown in Fig. 3. The circuit consists of the lens camera (ESP 32 CAM),
infrared-proximity  sensors  and  laser  light,  as  indicator.  The  two
infrared-proximity sensors were each placed on both sides of the camera
and equidistant from the camera. A blue light-emitting diode (LED) light
beam was implemented as the camera’s flashlight. The ESP 32 CAM is
also power-saving as it has the capacity to go into sleep mode when no
finger  is  detected  by  the  infrared-proximity  sensors.  The  contactless
scanning device can be powered via a USB charger or the USB port of a
personal computer.

2.3. The programming of the device and webpage interface

The contactless fingerprint scanner microcontroller (Aithinker ESP
32 WROOM) was programmed in C-language, using the Arduino IDE.
The webpage interface was designed using HTML and CSS as shown in
Fig. 4. The WiFi capability of the ESP 32 CAM enabled the connectivity
of the device to smartphones, laptops and all WiFi-enabled devices. The
operation of the contactless fingerprint biometric scanner followed the
algorithm in steps. The device implements the pattern recognition al-
gorithms in machine learning language (MLL); in which the fingerprint
is first enrolled and stored in a database, where any scanned fingerprint
data can be sampled and matched within the verification stage. Python
language was deployed in the programming.

The scanner used the images (pictures) captured by the camera to

identify the different fingers, just like the traditional fingerprint scan-
ners.  The  images  were  stored  and  processed  using  image  recognition
algorithms similar to the ones used in face recognition. Each image file
was converted into a binary file, which was then analyzed pixel by pixel.
Whenever a finger is scanned, the system searches the database for a
match.  The  more  the  samples  of  a  particular  fingerprint  stored,  the
better and more robust the results of the scan.

Once  the  device  is  switched  on,  it  searches  for  an  available
programme-compatible WiFi device to pair and continues to search for a
WiFi-enabled device if none is available. On securing a connection, it
waits  for  the  infrared-proximity  sensors  to  detect  a  target  finger  for
scanning. Once a finger is detected at the predetermined distance from
the  camera,  the  detected  finger  is  scanned.  It  then  stores  the  newly
captured  fingerprint  data  in  the  memory  storage  device  (when  the
ENROLL button is enabled from the webpage; cf. Fig. 4) for later use. On
the other hand, when the enroll button is not selected (from the web-
page),  the  device  compares  the  captured  fingerprint  images  with  the
previously stored fingerprint data, computes the correlation factor from
the sample and displays the value of the achieved correlation factor for
analysis.

2.4. WiFi-enablement for enrollment and verification

The device was designed to match a high population of users needing
its services since multiple-user enrollment and verification devices are
known to be ideal for authentication purposes in public domains with
high  population  indices  [35].  To  ensure  easy  availability  and  fast
turn-around-times during the verification process, the microcontroller
Aithinker  ESP32  WROOM  was  implemented.  It  enables  the  WiFi

Array11(2021)1000833U.I. Oduah et al.

Fig. 4. Shows a graphical user-interface of the contactless fingerprint biometric
scanner. The scanner is WiFi-enabled and can connect to WiFi-enabled devices,
such as mobile phones, ipads, personal computers and laptops.

sensors go into sleep mode when there is no finger in scanning view. The
inactive mode of the sensors conserves electric power, making the de-
vice energy efficient.

3. Results

Varying  the  set  distances  between  the  finger  and  camera  was
observed to affect the values of the correlation factor, in agreement with
previous reports [34]. Consequently, to determine the ideal distance of
the  target  finger  from  the  camera  during  scanning,  the  two
infrared-proximity sensors were set-up to detect at different distances
between the camera and target finger, and the correlation factors for the
distances were recorded (cf. Table 1). The contactless scanner was able
to capture fingerprint images at distances within 0.5–9.5 cm, from the
camera to the target finger, with the highest correlation factor of 59.9%
at a distance of 3.5 cm from the camera.

Furthermore,  it  was  noticed  that  the  achieved  correlation  factor
varied slightly with the intensity of the illuminating background light.
Therefore, the effect on the correlation factor was tested by applying a
light beam of different intensities (50–280 lm) at 3.5 cm (Table 2). The
results  indicate  that  the  higher  the  light  intensity,  the  better  the
fingerprint  image  resolution,  which  in  turn  enhances  the  correlation
factor.

It is well-known that the percentages of light reflected, transmitted
and/or absorbed contribute to the resolutions of  the images obtained
during fingerprint scanning and these, in turn, influence the correlation

Table 1
Determination of the ideal distance from camera lens to target finger for highest
correlation factor.

Correlation

Factor (%)

Distance (cm)

0.5

2.0

3.5

5.0

6.5

8.0

9.5

47.57
49.05
46.84
46.64
44.33

54.99
54.64
56.07
51.02
53.74

55.13
55.48
58.07
59.90
55.25

55.56
45.16
47.52
50.76
47.51

49.22
52.75
51.26
45.35
43.22

46.18
49.71
51.11
45.74
42.13

51.47
47.19
47.95
54.77
49.48

Fig.  3. Schematic  of  the  layout  of  the  electronic  components  on  the  circuit
board. IRL1 and IRL2 are infrared-proximity sensors. L1 is a laser point beam,
which indicates that the target finger is properly aligned before scanning. EPS-
325  CAM  is  the  lens  camera;  It  captures  the  image  of  the  fingerprint  at  the
prompt of the microcontroller. U1 and U2 are transistors in the microcontroller
that performs the logic gate functions. When the target finger is sensed by the
two infrared-proximity sensors at a fixed vertical and horizontal distance away
from  the  camera,  the  laser  point  beam  switches  on  and  the  camera  simulta-
neously captures the image. The microcontroller performs the logic functions
controlling  the  two  infrared-proximity  sensors,  lens  camera  and  laser
point beam.

connection  of  the  contactless  fingerprint  biometric  scanner  to  other
WiFi-enabled devices, such as mobile phones, ipads, laptops, personal
computers, etc. A snapshot of the webpage is shown in Fig. 4, displaying
the internet protocol (IP).

2.5. Packaging of the device

The packaging of the developed device is robust and compact, with
an  opening  on  top  to  guide  the  user  on  the  positioning  of  the  target
finger. The fingerprint scanner is delineated in Fig. 2(a) with a dimen-
sion of 14 cm (length) by 12 cm (width) by 7 cm (height) in size. This is
different from current approaches, with openings to let the target finger
inside a casing with a physical framework to confine the finger during
scanning  [36].  The  contactless  fingerprint  scanner  herein  has  been
designed in such a manner that the target finger is neither confined nor
in  contact  with  the  scanner’s  architecture.  The  scanner  is  positioned
separately  from  where  the  finger  is  aligned,  in  open  space,  to  the
camera.

The coupling of the electronic components on the circuit board was
also implemented with the image-capturing components properly posi-
tioned  at  points  where  they  can  function  without  obstructions.  The
circuit diagram layout is presented in Fig. 3. The two infrared-proximity

Array11(2021)1000834U.I. Oduah et al.

Table 2
Determination  of  ideal  luminous  flux  using  white  light  at  ideal  distance  for
highest correlation factor.

Correlation Factor

(%)

Light Intensity (lm)

L0 = 50

51.43
49.55
43.73
49.07
49.55

L3 =
150

51.04
50.28
60.48
57.48
56.20

L6 =
180

58.36
56.91
66.95
63.91
50.21

L9 =
210

60.77
57.81
63.22
58.00
61.54

L12 =
280

78.12
73.05
69.98
75.80
72.73

factors obtained [34]. Nonetheless, light irradiance at long wavelengths
is absorbed by the epidermis. To ameliorate skin damage, blue light (λ =
500 nm), which is less absorbed by the skin, was implemented for the
background illumination at 25 lumens at the 3.5 cm distance. The results
are collected in Table 3.

Blue  light  at  280 lumens was  also  tested  to examine its  impact  at
higher intensity. Gratifyingly, the highest correlation factor of 97.51%
was obtained using blue light of 500 nm at 280 lumens intensity and a
3.5 cm distance between the target finger and lens camera (cf. Table 3).

4. Discussion

The high-precision contactless fingerprint biometric scanner devel-
oped in this project overcomes the challenge of positioning the target
finger  at  a  fixed  point  during  scanning,  without  contact.  Previous
technologies  implemented  finger  placement  guides  to  achieve  the
alignment of the target finger at a fixed position but with the possibility
of  the finger touching  the placement  guides, leading to  the spread of
contagious infections. Other unconstrained acquisition setups are faced
with problems of motion blur and inconstant fingerprint image resolu-
tions  [37,38].  Herein,  two  infrared-proximity  sensors  have  been
deployed with an electronic logic-controlled microcontroller to achieve
the non-touch alignment of the target finger in a fixed constant position
away from the camera.

The distance between the target finger and lens camera during scan
operations was found to be optimal at 3.5 cm, with a correlation factor of
59.9% (Table 1). Other factors that influence the correlation factor of
the contactless fingerprint scanners were also investigated, with a view
to  improving  it.  For  instance,  most  contactless  fingerprint  scanners
implement  diffused  white  light  for  background  illumination.  This  is
reported  to  produce  a  uniform  but  low  resolution,  which  results  in  a
reduction in the correlation factor [39]. The background light intensity
was varied between 50 lm and 280 lm in this study (Table 2). It is also
significant to note that the luminous flux range of the samples investi-
gated  was  within  that  of  liquid  crystal  display  (LCD)  units  in  mobile
phones  and  laptops.  The  highest  fingerprint  correlation  factor  was
achieved at 280 lm, with a correlation factor of 78.12%. It is evident that
the increased light intensity enhanced the resolution of the fingerprint

Table 3
Determination of the ideal luminous flux using blue light (500 nm) at the ideal
distance (3.5 cm) for the highest correlation factor.

Correlation Factor (%)

Blue Light (lm)

25

81.17
87.08
89.51
72.82
71.85
75.80
80.35
80.88
75.02
89.14

280

89.17
95.08
97.51
80.82
79.85
83.80
88.35
88.88
85.02
95.14

images, resulting in their improved correlation factors.

Cognizant  of  the  reports  that  white  light  and  other  longer  wave-
length radiations are absorbed by the epidermal layer of the skin [40,
41], white light was substituted with blue light (500 nm), which is less
absorbed by the skin, and its impact on correlation factor was assessed.
A corollary is that the blue illuminating light enhanced the correlation
factors of the fingerprint images, with the highest value at 97.51%. The
excellent values obtained for the correlation factors using blue light can
be  attributed  to  the  high-resolution  fingerprint  images  generated,
which, due to reduced absorption by the skin, more clearly outlined the
ridges and valleys of the target finger. The import of this wavelength
change from illuminating white to blue light is, therefore, not only an
improved  correlation  factor  but  also  a  device  benign  to  the  skin  and
environmentally friendly.

Fig. 5 compares the correlation factors obtained using blue illumi-
nating light (λ  = 500 nm) at both low (25 lm) and high (280 lm) in-
tensities (cf. Table 3), highlighting the fact that increased light intensity
enhances correlation factor by improving the resolution of the finger-
print images. This was similarly observed with white light (cf. Table 2).
The  robustness  of  this  fingerprint  scanner  is  demonstrated  with
variations  in  the  intensities  of  the  illuminating  background  light  be-
tween 50 lm and 280 lm, and variations of the luminous flux for blue
light  at  500  nm.  The  developed  fingerprinting  device  delivered
improved correlation factors and was observed to perform best when the
target finger was placed at a vertical distance of 3.5 cm away from the
camera lens, applying blue light of 500 nm at an intensity of 280 lm.

The correlation factor values obtained when using white light of 280
lm  intensity  for  background  illumination  in  our  model  contactless
fingerprint  scanner,  with  an  optimal  distance  of  3.5  cm  between  the
target  finger  and  lens  camera,  was  also  compared  to  those  generated
using blue light at both 25 lm and 280 lm (Fig. 6). Overall, blue light
proved  better  resolving,  with  higher  correlation  factor  values,  than
white light at 280 lm intensity although the values of the correlation
factor obtained with white light of 280 lm were comparable to those of
25 lm blue illuminating light.

One of the major concerns of fingerprint authentication is the issue of
security  and  privacy.  The  fingerprint  biometric  scanners  can  be
vulnerable  to  such system  failures  as  intrinsic  failures, which  include
failures caused by faulty sensors and inaccurate sample matching as well
as  failures  due  to  fingerprint  features  extraction  or  false  errors  [42].
Non-intrinsic  failures,  on  the  other  hand,  are  mainly  attributable  to
malicious  attacks.  Therefore,  the  protection  of  the  fingerprint  data
during  the  acquisition,  storage,  sampling  and  verification  exercises  is
paramount.  Pertinently,  the  fingerprint  acquisition  techniques  imple-
mented  herein  eliminate  the  risks  associated  with  the  fraudulent
acquisition  of  latent  fingerprints.  The  security  concepts  and  methods
implemented  to  address  fingerprint  data  confidentiality,  authenticity
and integrity as well as its diversity, non-repudiation and revocability in
the software development of this device will be reported elsewhere.

5. Conclusions

In  this  study,  a  new  approach  to  fingerprint  image  acquisition
technology using a system that enforces contactless fingerprinting in a
precise  and  efficient  manner  has  been  successfully  demonstrated.  It
generates great potentials in the replacement of the conventional touch-
fingerprint scanners and is especially apropos in the wake of the pre-
vailing COVID-19 pandemic to mitigate the spread of the virus and other
contagious infections transmitted through surfaces.

The  findings  suggest  best  techniques  towards  achieving  optimal
fingerprint correlation factor values with reduced false positive errors.
The contactless fingerprint biometric scanner developed herein is pres-
ently at its proof-of-concept stage and a requisite patent has been filed in
Nigeria (with registration number NG/PT/NC/2020/4853).

It is vital to point out that in order to ensure the compatibility of the
contactless  fingerprint  biometric  systems  with  already-existing  touch-

Array11(2021)1000835U.I. Oduah et al.

Fig. 5. Determination of the optimum luminous flux using blue light (500 nm) at intensities of 25 lm and 280 lm.

Fig. 6. Comparative luminous fluxes using blue light at intensities of 25 lm and 280 lm, and white light at 280 lm.

based equivalent fingerprint images, the device developed in this work
possesses  the  unique  feature  of  producing  2-D  images  with  improved
correlation  factors. It is common knowledge that fingerprint scanners
that operate by using the 3-D scene reconstruction of images suffer the
challenges of occlusion, scale and light variations, and perspective dis-
tortions, which reduce their correlation factors especially when applied
in  the  verification  of  2-D  images  already  captured  with  touch-based
fingerprint scanners.

To  surmise,  high-precision  contactless  fingerprint  scans  and
enhanced correlation factors have been achieved in this protocol as well
as  an  effective,  efficient  and  convenient  fingerprinting  architectural
technique, which not only bodes well for mitigating the 3-D/2-D image
resolution  dichotomy  but  also  the  spread  of  contagions,  such  as  the

SARS-CoV-2.

Declaration of competing interest

The authors declare that they have no known competing financial
interests or personal relationships that could have appeared to influence
the work reported in this paper.

References

[1] Kumar A. Towards more accurate matching of contactless palmprint images under
less constrained environments. IEEE Trans Inf Forensics Secur 2019;14:34–47.
[3] Lai C-C, Shih T-P, Ko W-C, Tang H-J, Hsueh P-R. Severe acute respiratory syndrome
coronavirus 2 (SARS-CoV-2) and coronavirus disease-2019 (COVID-19): the

Array11(2021)1000836U.I. Oduah et al.

epidemic and the challenges. Int J Antimicrob Agents 2020;55:105924. https://
doi.org/10.1016/j.ijantimicag.2020.105924.

[4] Patil S, Sheelvant S. Survey on image quality assessment techniques. Indian J Sci

[28] Feng L, David Z. 3D fingerprint reconstruction system using feature

correspondences and prior estimated finger model. Pattern Recogn 2014;47:
178–93.

Res 2015;4:1756–9.

[5] Sampat MP, Wang Z, Gupta S, Bovik AC, Markey MK. Complex wavelet structural
similarity: a new image similarity index. IEEE Trans Image Process 2009;18:
2385–401.

[6] Hildebrandt M, Dittmann J, Vielhauer C. Statistical latent fingerprint residue in
contactless scans to support fingerprint segmentation. In: 18th  international
conference on digital signal processing (DSP); 2013. p. 1–6. Santorini, Greece.
[7] Fiumara G, Flanagan P, Schwarz M, Tabassi E, Boehnen C. NIST special database

301: nail to nail fingerprint challenge dry run. Natl. Inst. Stand. Technol. Tech.
Note 2018:22pp. https://doi.org/10.6028/NIST.TN.2002. 2002.

[8] Oduah UI. The application of photorefractive effect of Lithium Niobate in the

development of a biometric fingerprint scanner with unique sensitivity. Int J Electr
Comput Eng 2014;6:236–9.

[9] Chen Y, Parziale G, Diaz-Santana E, Jain AK. 3D touchless fingerprints:

compatibility with legacy rolled images. In: Proceedings of bio-metric symposium.
Baltimore, MD: Biometric Consortium Conference; 2006. p. 1–6.

[10] Ericson L, Shine S. Evaluation of contactless versus contact fingerprint data, phase
2 (version 1.1). Fairmont, WV.: DOJ Office of Justice Programs, National Institute
of Justice; 2015. p. 1–117.

[11] Donida-Labati R, Genovese A, Piuri V, Scotti F. Toward unconstrained fingerprint

recognition: a fully touchless 3-D system based on two views on the move. IEEE
Trans. Syst. Man Cybern. Syst. 2016;46:202–19.

[12] Feng J. Combining minutiae descriptors for fingerprint matching. Pattern Recogn

2008;41:342–52.

[13] Agrawal A, Raskar R, Chellappa R. What is the range of surface reconstructions

[29] Song Y, Lee C, Kim J. A new scheme for touchless fingerprint recognition system.
In: Ko SJ, editor. Proceedings of 2004 international symposium on intelligent
signal processing and communication systems (ISPACS 2004). Seoul, Korea:
ISPACS; 2004. p. 524–7.

[30] Boult T. Beyond image quality: failure analysis from similarity surface techniques.

Gaithersburg, MD: NIST Biometric Quality Workshop; 2006. p. 1–23.

[31] Zheng Q, Kumar A, Pan G. Contactless 3D fingerprint identification without 3D

reconstruction. Sassari, Italy: International Workshop on Biometrics and Forensics
(IWBF); 2018. p. 1–6.

[32] Liu X, Pedersen M, Charrier C, Cheikh FA, Bours P. An improved 3-step contactless
fingerprint image enhancement approach for minutiae detection. In: Proceedings
of the 6th European workshop on visual information processing (EUVIP). Marseille,
France: EUVIP; 2016. p. 1–6.

[33] Donida-Labati R, Genovese A, Piuri V, Scotti F. Contactless fingerprint recognition:
a neural approach for perspective and rotation effects reduction. In: Proceedings of
the IEEE workshop on computational intelligence in biometrics and identity
management (CIBIM 2013). Singapore: IEEE; 2013. p. 22–30.

[34] Liu F, Zhang D, Song C, Lu G. Touchless multiview fingerprint acquisition and

mosaicking. IEEE Trans. Instrum. Meas. 2013;62:2492–502.

[35] Derawi MO, Yang B, Busch C. Fingerprint recognition with embedded cameras on
mobile phones. In: Prasad R, Farkas K, Schmidt AU, Lioy A, Russello G, Luccio FL,
editors. Security and privacy in mobile information and communication systems
(MobiSec 2011). Computer sciences, social informatics and telecommunications
engineering, vol. 94. Berlin Heidelberg: Springer; 2012. 136–147.. https://doi.org/
10.1007/978-3-642-30244-2_12.

from a gradient field?. In: Leonardis A, Bischof H, Pinz A, editors. Computer vision
– ECCV 2006 lecture notes in computer science, vol. 3951. Berlin Heidelberg:
Springer-Verlag; 2006. p. 578–91.

[36] Wang Y, Hassebrook LG, Lau DL. Noncontact depth-detailed 3D fingerprinting.
SPIE Newsroom 2009:1–3. https://doi.org/10.1117/2.1200912.002552.
[37] Parziale G, Diaz-Santana E, Hauke R. The Surround ImagerTM: A multi-camera

[14] Gamassi M, Piuri V, Scotti F. Fingerprint local analysis for high-performance

minutiae extraction. In: Proceedings of the 2005 IEEE international conference on
image processing (ICIP 2005); 2005. p. 265–8. Genoa, Italy.

[15] Kumar A, Kwong C. Towards contactless, low-cost and accurate 3D fingerprint

identification. In: Proceedings of the 2013 IEEE conference on computer vision and
pattern recognition workshops (CVPRW ’13). Portland, OR: IEEE Computer
Society; 2013. p. 3438–43.

[16] Jain AK, Prabhakar S, Hong L, Pankanti S. Filter bank-based fingerprint matching.

IEEE Trans Image Process 2000;9:846–59.

[17] Yin X, Zhu Y, Hu J. Contactless fingerprint recognition based on global minutia

topology and loose genetic algorithm. IEEE Trans Inf Forensics Secur 2019;15:
28–41.

[19] Maltoni D, Maio D, Jain AK, Prabhakar S. Handbook of fingerprint recognition. 2nd

ed. London: Springer; 2009.

[21] Donida-Labati R, Genovese A, Piuri V, Scotti F. Accurate 3D fingerprint virtual

environment for biometric technology evaluations and experiment design. In:
Proceedings of the 2013 IEEE international conference on computational
intelligence and virtual environments for measurement systems and applications
(CIVEMSA). Milan, Italy: IEEE; 2013. p. 43–8. https://doi.org/10.1109/
CIVEMSA.2013.6617393.

[22] Mil’shtein S, Pillai A. Perspectives and limitations of touchless fingerprint. In: 16th
IEEE international symposium on technologies for homeland security (HST ’17).
Waltham, MA.: IEEE; 2017. p. 1–6.

touchless device to acquire 3D rolled-equivalent fingerprints. In: Zhang D, Jain AK,
editors. Advances in biometrics (ICB 2006), lecture notes in computer science, vol.
3832. Berlin Heidelberg: Springer; 2005. p. 244–50. https://doi.org/10.1007/
11608288_33.

[38] Khutlang R, Khanyile N, Makinana S, Nelwamondo F. High resolution feature

extraction from optical coherence tomography acquired internal fingerprint.
Proceedings of the 17th  IEEE/ACIS international conference in software
engineering, artificial intelligence, networking and parallel/distributed computing.
Shanghai, China: IEEE/ACIS; 2016. p. 637–41. https://doi.org/10.1109/
SNPD.2016.7515971.

[39] Donida-Labati R, Genovese A, Piuri V, Scotti F. Measurement of the principal

singular point in contact and contactless fingerprint images by using computational
intelligence techniques. In: Proceedings of the 2010 IEEE international conference
on computational intelligence for measurement systems and applications (CIMSA
2010). Taranto, Italy: IEEE; 2010. p. 18–23.

[40] Kumar A, Zhou Y. Contactless fingerprint identification using level zero features.
In: 2011 IEEE computer society conference on computer vision and pattern
recognition workshops (CVPR workshops 2011); 2011. 114–119.. https://doi.org/
10.1109/CVPRW.2011.5981823. IEEE, Colorado Springs, CO.

[41] Oduah UI, Yang W. Advanced photodetector chip. IEEE Sensor J 2016;16:5610–7.
[42] Zhao F, Tang X. Preprocessing and post-processing for skeleton-based fingerprint

minutiae extraction. Pattern Recogn 2007;40:1270–81.

[43] Zhang D, Liu F, Zhao Q, Lu G, Luo N. Selecting a reference high resolution for

[23] Donida-Labati R, Genovese A, Piuri V, Scotti F. Touchless fingerprint biometrics: A

survey on 2D and 3D technologies. J Internet Technol 2014;15:327–34.

fingerprint recognition using minutiae and pores. IEEE Trans. Instrum. Meas. 2011;
60:863–71.

[24] Zhou W, Hu J, Petersen IR, Wang S, Bennamoun M. A benchmark 3D fingerprint
database. In: Proceedings of the 11th international conference on fuzzy systems and
knowledge discovery (FSKD 2014); 2014. p. 935–40. Xiamen, China.

[44] Zhao X, Evangelopoulos G, Chu D, Shah S, Kakadiaris AI. Minimizing illumination
differences for 3D to 2D face recognition using lighting maps. IEEE Trans. Cybern.
2014;44:725–36.

[25] Wang Y, Lau DL, Hassebrook LG. Fit-sphere unwrapping and performance analysis

[45] Wang Y, Hassebrook LG, Lau DL. Data acquisition and processing of 3-D

of 3D fingerprints. Appl Opt 2010;49:592–600.

[26] Tang Y, Jiang L, Hou Y, Wang R. Contactless fingerprint image enhancement
algorithm based on hessian matrix and STFT. 2nd  international conference on
multimedia and image processing (ICMIP 2017). Wuhan, China: ICMIP; 2017.
p. 156–60.

fingerprints. IEEE Trans. Inf. Forensics Secur. 2010;5:750–60.

[46] Xiao Q. Technology review: biometrics-technology application challenge and

computational intelligence solutions. IEEE Comput. Intell. Mag. 2007;2(2):5–25.

Array11(2021)1000837