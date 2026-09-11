# =============================================================================
#  발표자료 HTML -> A4 가로 Word(.docx) 변환
#  python-docx로 직접 구성한다. HTML을 자동 변환하면 페이지 설정과 표 서식이
#  깨지므로, 문서 구조를 그대로 옮기되 Word 관례에 맞게 재조판한다.
# =============================================================================
from docx import Document
from docx.shared import Pt, Mm, RGBColor, Inches
from docx.enum.section import WD_ORIENT
from docx.enum.text import WD_ALIGN_PARAGRAPH
from docx.enum.table import WD_TABLE_ALIGNMENT
from docx.oxml.ns import qn
from docx.oxml import OxmlElement

KR   = '맑은 고딕'
MONO = 'Consolas'
SERIF= 'Cambria'

INK   = RGBColor(0x12,0x18,0x1c)
BODY  = RGBColor(0x33,0x3e,0x45)
MUTED = RGBColor(0x66,0x75,0x7e)
ACC   = RGBColor(0x0f,0x6f,0x8c)
WARN  = RGBColor(0xa8,0x50,0x0f)

doc = Document()

# ---- A4 가로 ----
# [주의] python-docx는 orientation만 바꿔도 실제 크기를 바꾸지 않는다.
#        page_width / page_height 를 직접 맞바꿔야 한다.
sec = doc.sections[0]
sec.orientation   = WD_ORIENT.LANDSCAPE
sec.page_width    = Mm(297)
sec.page_height   = Mm(210)
sec.left_margin   = Mm(22)
sec.right_margin  = Mm(22)
sec.top_margin    = Mm(18)
sec.bottom_margin = Mm(16)
CONTENT_MM = 297 - 44          # 253mm

def set_font(run, name=KR, size=10.5, bold=False, color=BODY, italic=False):
    run.font.name = name
    run.font.size = Pt(size)
    run.bold = bold
    run.italic = italic
    run.font.color.rgb = color
    # 한글은 eastAsia 폰트를 따로 지정해야 적용된다.
    # [주의] Consolas/Cambria 같은 라틴 전용 서체를 eastAsia에 그대로 넣으면
    # 그 런 안에 섞인 한글이 Word 기본 대체 서체로 떨어져 서식이 흐트러진다.
    # 라틴 서체를 쓰더라도 eastAsia는 항상 한글 서체로 고정한다.
    run._element.rPr.rFonts.set(qn('w:eastAsia'), KR)

def shade(cell, hexcolor):
    el = OxmlElement('w:shd')
    el.set(qn('w:val'), 'clear'); el.set(qn('w:fill'), hexcolor)
    cell._tc.get_or_add_tcPr().append(el)

def cell_text(cell, text, size=8.5, bold=False, color=BODY, name=KR, align=None):
    cell.text = ''
    p = cell.paragraphs[0]
    p.paragraph_format.space_before = Pt(2); p.paragraph_format.space_after = Pt(2)
    if align is not None: p.alignment = align
    set_font(p.add_run(text), name, size, bold, color)

def para(text, size=10.5, bold=False, color=BODY, name=KR,
         before=0, after=6, indent=0, align=None, italic=False):
    p = doc.add_paragraph()
    pf = p.paragraph_format
    pf.space_before = Pt(before); pf.space_after = Pt(after)
    pf.line_spacing = 1.32
    if indent: pf.left_indent = Mm(indent)
    if align is not None: p.alignment = align
    set_font(p.add_run(text), name, size, bold, color, italic)
    return p

def rule(color='D8E0E4', size=6):
    p = doc.add_paragraph()
    p.paragraph_format.space_before = Pt(2); p.paragraph_format.space_after = Pt(8)
    pPr = p._p.get_or_add_pPr()
    b = OxmlElement('w:pBdr'); bt = OxmlElement('w:bottom')
    bt.set(qn('w:val'),'single'); bt.set(qn('w:sz'),str(size))
    bt.set(qn('w:space'),'1'); bt.set(qn('w:color'),color)
    b.append(bt); pPr.append(b)

def section_head(num, title):
    p = doc.add_paragraph()
    p.paragraph_format.space_before = Pt(16); p.paragraph_format.space_after = Pt(7)
    p.paragraph_format.keep_with_next = True
    set_font(p.add_run(num + '  '), MONO, 11, True, ACC)
    set_font(p.add_run(title), KR, 15, True, INK)

def quote(text, cite):
    p = doc.add_paragraph()
    pf = p.paragraph_format
    pf.left_indent = Mm(6); pf.right_indent = Mm(6)
    pf.space_before = Pt(6); pf.space_after = Pt(2); pf.line_spacing = 1.25
    pPr = p._p.get_or_add_pPr()
    b = OxmlElement('w:pBdr'); lf = OxmlElement('w:left')
    lf.set(qn('w:val'),'single'); lf.set(qn('w:sz'),'18')
    lf.set(qn('w:space'),'8'); lf.set(qn('w:color'),'0F6F8C')
    b.append(lf); pPr.append(b)
    sh = OxmlElement('w:shd'); sh.set(qn('w:val'),'clear'); sh.set(qn('w:fill'),'E3EFF3')
    pPr.append(sh)
    set_font(p.add_run(text), SERIF, 10, False, INK, italic=False)
    c = doc.add_paragraph()
    c.paragraph_format.left_indent = Mm(6); c.paragraph_format.space_after = Pt(8)
    set_font(c.add_run('— ' + cite), MONO, 8, False, MUTED)

def bullets(items):
    for it in items:
        p = doc.add_paragraph()
        pf = p.paragraph_format
        pf.left_indent = Mm(6); pf.first_line_indent = Mm(-5)
        pf.space_after = Pt(4); pf.line_spacing = 1.3
        set_font(p.add_run('▲  '), KR, 8, False, ACC)
        # **굵게** 마크업 처리
        for i, seg in enumerate(it.split('**')):
            if seg: set_font(p.add_run(seg), KR, 10, i % 2 == 1,
                             INK if i % 2 == 1 else BODY)

def table(headers, rows, widths_mm, caption=None, hl_rows=(), mono_cols=(), ref_cols=()):
    t = doc.add_table(rows=1, cols=len(headers))
    t.style = 'Table Grid'
    t.alignment = WD_TABLE_ALIGNMENT.CENTER
    t.autofit = False
    for i, h in enumerate(headers):
        c = t.rows[0].cells[i]
        c.width = Mm(widths_mm[i])
        shade(c, 'EEF2F4')
        cell_text(c, h, 8, True, MUTED)
    for r_i, row in enumerate(rows):
        cells = t.add_row().cells
        for i, v in enumerate(row):
            cells[i].width = Mm(widths_mm[i])
            if r_i in hl_rows: shade(cells[i], 'E3EFF3')
            nm = MONO if (i in mono_cols or i in ref_cols) else KR
            col = ACC if i in ref_cols else BODY
            al = WD_ALIGN_PARAGRAPH.RIGHT if i in mono_cols else None
            cell_text(cells[i], v, 8.5, False, col, nm, al)
    if caption:
        p = doc.add_paragraph()
        p.paragraph_format.space_before = Pt(3); p.paragraph_format.space_after = Pt(10)
        set_font(p.add_run(caption), KR, 8, False, MUTED)
    return t

def measures(pairs):
    t = doc.add_table(rows=2, cols=len(pairs))
    t.style = 'Table Grid'
    t.alignment = WD_TABLE_ALIGNMENT.CENTER
    w = CONTENT_MM / len(pairs)
    for i, (k, v) in enumerate(pairs):
        kc, vc = t.rows[0].cells[i], t.rows[1].cells[i]
        kc.width = Mm(w); vc.width = Mm(w)
        shade(kc, 'EEF2F4')
        cell_text(kc, k, 8, False, MUTED, KR, WD_ALIGN_PARAGRAPH.CENTER)
        cell_text(vc, v, 14, True, INK, MONO, WD_ALIGN_PARAGRAPH.CENTER)
    doc.add_paragraph().paragraph_format.space_after = Pt(4)

def finding(tag, title, body_text, delta):
    p = doc.add_paragraph()
    p.paragraph_format.space_before = Pt(8); p.paragraph_format.space_after = Pt(3)
    p.paragraph_format.keep_with_next = True
    set_font(p.add_run(f'[{tag}]  '), MONO, 9, True, WARN)
    set_font(p.add_run(title), KR, 11.5, True, INK)
    for i, seg in enumerate(body_text.split('**')):
        pass
    q = doc.add_paragraph()
    q.paragraph_format.left_indent = Mm(4); q.paragraph_format.space_after = Pt(4)
    q.paragraph_format.line_spacing = 1.3
    for i, seg in enumerate(body_text.split('**')):
        if seg: set_font(q.add_run(seg), KR, 10, i % 2 == 1,
                         INK if i % 2 == 1 else BODY)
    d = doc.add_paragraph()
    d.paragraph_format.left_indent = Mm(4); d.paragraph_format.space_after = Pt(9)
    pPr = d._p.get_or_add_pPr()
    sh = OxmlElement('w:shd'); sh.set(qn('w:val'),'clear'); sh.set(qn('w:fill'),'EEF2F4')
    pPr.append(sh)
    set_font(d.add_run('  ' + delta + '  '), MONO, 9.5, False, INK)

# =============================================================================
#  표지
# =============================================================================
p = doc.add_paragraph(); p.paragraph_format.space_after = Pt(3)
set_font(p.add_run('논문 구현 검증 리포트'), MONO, 9.5, True, ACC)

p = doc.add_paragraph(); p.paragraph_format.space_after = Pt(8)
set_font(p.add_run('비접촉 지문인식 논문을 Windows 네이티브 앱으로 구현하다'), KR, 24, True, INK)

para('체계적 문헌고찰 한 편을 골라 그 안의 고전 파이프라인을 C++20으로 구현했다. '
     '각 처리 단계가 논문의 어느 주장을 구현한 것인지 코드가 직접 들고 있고, '
     '실행할 때마다 실측값과 대조한 결과를 문서로 출력한다.', 11.5, after=10)

t = doc.add_table(rows=1, cols=3); t.autofit = False
meta = [('대상 논문', 'Chowdhury & Imtiaz (2022), J. Cybersecur. Priv. 2(3):714–730'),
        ('구현', 'C++20 · 4,875 LOC · 커밋 28'),
        ('플랫폼', 'Windows (Win32 + Media Foundation + OpenCV)')]
for i,(k,v) in enumerate(meta):
    c = t.rows[0].cells[i]; c.width = Mm(CONTENT_MM/3); c.text=''
    pp = c.paragraphs[0]; pp.paragraph_format.space_after = Pt(1)
    set_font(pp.add_run(k), KR, 8, True, MUTED)
    pp2 = c.add_paragraph(); pp2.paragraph_format.space_after = Pt(0)
    set_font(pp2.add_run(v), KR, 9, False, BODY)
rule('12181C', 12)

# =============================================================================
section_head('01', '한 장 요약')
para('리뷰 논문은 비접촉 지문인식의 획득·전처리·딥러닝을 정리하면서, 고전 방식이 비접촉 데이터에서 '
     '어디까지 무너지는지를 수치로 보고한다. 이 구현은 그 고전 경로를 그대로 재현하고, 논문이 예측한 '
     '취약성까지 함께 재현됐다. 구현 실패가 아니라 재현 성공이며, 논문이 제시한 해법(딥 세그멘테이션·'
     'MinuNet·Siamese)이 왜 필요한지를 자체 수치로 뒷받침하는 근거를 확보한 것이 이 단계의 성과다.')
measures([('논문 → 구현 매핑','12개 스택'),('단위·통합 테스트','30 / 30'),
          ('실측으로 발견한 결함','17건'),('종단 처리 시간','230 ms')])
bullets([
 '**논문 추적성을 코드에 내장했다.** 각 스택이 논문의 절·원문 인용·원저를 들고 있고, 실행 시 실측값과 대조한 결론을 생성한다. 문서와 코드가 따로 노는 것을 구조로 막았다.',
 '**정적 리뷰로는 못 잡는 결함을 실측으로 찾았다.** 30개 테스트를 모두 통과한 코드가, 사진 한 장을 5° 돌리자 인식 점수가 26배 무너졌다.',
 '**논문이 비운 자리를 채웠다.** 리뷰는 정확도만 보고한다. 인증 시스템을 자처하려면 EER·DET·CMC가 필요하다는 판단으로 지표 계산기를 따로 구현했다.',
])

# =============================================================================
doc.add_page_break()
section_head('02', '논문 구조와 구현 범위의 대응')
para('리뷰 논문은 세 가지 연구 질문으로 구성된다. 촬영 방법(§3), 고전 전처리(§4), 딥러닝(§5)이다. '
     '이 중 §3과 §4를 전면 구현하고, §5는 동일 인터페이스로 교체 가능한 고전 대응물을 구현해 기준선을 만들었다.')
table(
 ['논문 위치','논문의 내용','구현','코드','실측'],
 [
  ['§3 Table 2','2D 획득 장치 정리. NIST SP 500-305이 균일 조명·배경 분리·모션 저감을 요구','Media Foundation 카메라. 노출·초점·화이트밸런스 수동 고정','capture_mf.cpp','1,159 ppi'],
  ['§4 Table 3 ①','Finger Segmentation / Wang 2017 / YCbCr 피부색 추정','YCbCr 임계 + 최대 연결성분 + 주축 역회전','segmenter_ycbcr.cpp','마스크 72.1 %'],
  ['§4 Table 3 ②','회전·피치 주축 추정 / Zaghetto 2015 / 인공신경망','minAreaRect 주축 + 해부학적 상하 판별로 180° 모호성 해소','segmenter_ycbcr.cpp','주축 −4.9°'],
  ['§4 Table 3 ③','저대비 / Wang 2016 / CLAHE 및 확장','밴드패스 + 국소 RMS 정규화. CLAHE로는 음영이 제거되지 않아 교체','enhancer_gabor.cpp','coherence 0.989'],
  ['§4 Table 3 ④','센서 거리 ↔ 융선 주파수 / Zaghetto 2017 / 주파수맵','융선 주기가 9 px가 되도록 스케일 정규화','enhancer_gabor.cpp','주기 8.55 px'],
  ['§4 Table 3 ⑤','코어·특이점 검출 / Labati 2010 / Poincaré 방향장 분석','구조텐서 배각 공간 방향장','enhancer_gabor.cpp','—'],
  ['§4 Enhancement','공간영역 + 주파수영역 향상 / Tang 2017 / Hessian + STFT','Gabor 필터뱅크. 가우시안(공간 국소성) × 정현파(주파수 선택성)','enhancer_gabor.cpp','융선 45.3 %'],
  ['§4 Minutiae','미뉴셔·융선골 추출·방향 추정을 머신러닝으로 다뤄야 한다','Crossing Number + 방향장 일관성 기반 품질 필터','minutiae_cn.cpp','107 개'],
  ['§5 MinuNet','ContactlessMinuNet: hourglass 인코더 + 위치(sigmoid)·방향(tanh) 2브랜치. 94.10 % (기존 89.61 %)','고전 대응물 구현. 동일 인터페이스라 ONNX 모델 준비 시 코드 변경 없이 교체','abstractions.hpp','미구현'],
  ['§5 Siamese','영상 쌍의 일치·불일치를 같은 망에서 학습','국소구조 서술자 + 강체변환 투표(RANSAC 계열)','matcher_minutiae.cpp','동일영상 1.000'],
  ['§5 Table 4','EER·rank-1로 성능 보고','EER·DET·CMC·AUC 계산기. 해석적 기대값으로 검증','metrics.cpp','테스트 12/12'],
  ['§6 Discussion','대규모 데이터셋 부재, 컴퓨팅 자원 등 6대 미해결 과제','범위 밖 항목을 명시하고 백로그로 이관','code_review.md','—'],
 ],
 [26, 78, 78, 40, 31],
 caption='논문의 각 항목이 어느 코드로 구현됐는지, 그리고 무엇이 실측됐는지',
 hl_rows=(2,4), ref_cols=(0,), mono_cols=(3,4))

# =============================================================================
doc.add_page_break()
section_head('03', '논문의 한 문장을 구현 결정으로 옮긴 사례')
para('표의 한 행을 읽고 무엇을 만들지 정하는 과정을 두 가지만 구체적으로 보인다. '
     '리뷰가 왜 두 항목을 한 행에 묶었는지를 이해했는가가 갈림길이었다.')
quote('Distance to the sensor, ridge line frequency — Frequency map, sensor-finger distance approximation.',
      '§4 Table 3, 원저 Zaghetto et al. 2017')
para('이 행은 촬영거리와 융선 주파수를 하나로 묶는다. 비접촉에서는 거리가 배율을 결정하고, 배율이 융선 '
     '주기를 통째로 이동시키기 때문이다. 같은 손가락도 주기가 4 px일 수도 20 px일 수도 있다. 그래서 고정 '
     '픽셀 크기로 리샘플하면 안 되고, 융선 주기를 기준으로 리샘플해야 한다는 결론이 나온다. 목표 주기를 '
     '9 px로 두고 영상을 다시 표본화하도록 구현했다. 부수 효과가 더 컸다 — 두 촬영본의 배율이 자동으로 '
     '정렬되어 미뉴셔 좌표가 배율 불변이 되고, 매칭이 안정된다.')
quote('To improve image contrast and sharpness, image enhancement techniques such as spatial domain '
      'techniques and frequency domain techniques can be used to improve the quality.',
      '§4 Fingerprint Image Enhancement, 원저 Tang et al. 2017 (Hessian matrix + STFT)')
para('공간영역과 주파수영역을 동시에 만족시키라는 요구다. Gabor 필터는 가우시안(공간 국소성)과 '
     '정현파(주파수 선택성)의 곱이므로 이 조건을 하나의 커널로 충족한다. 논문이 인용한 Hessian+STFT와 '
     '목적은 같고 수단이 다르다. 구현에서 가장 실수가 잦은 지점은 축 방향이었다 — 코사인이 융선의 법선을 '
     '따라 진동해야 융선·골 주기와 맞물린다. 융선 방향을 따라 진동시키면 정반대가 된다.')

# =============================================================================
doc.add_page_break()
section_head('04', '논문이 예측한 실패를 그대로 재현하다')
para('리뷰 Table 4는 비접촉↔접촉 교차 매칭에서 세 방식을 비교한다. 이 구현은 두 번째 축, 즉 고전 미뉴셔 '
     '매처의 자리에 있다. 실측된 취약성은 논문이 그 자리에 대해 보고한 내용과 일치한다.')
doc.add_picture('/tmp/chart_eer.png', width=Mm(230))
doc.paragraphs[-1].alignment = WD_ALIGN_PARAGRAPH.CENTER
para('출처: Chowdhury & Imtiaz (2022) Table 4. 본 구현은 맨 아래 칸의 자리에 해당한다. '
     '피험자 1명으로는 EER을 산출할 수 없어 자체 수치는 표시하지 않았다.', 8, color=MUTED, after=10)
para('자체 실측도 같은 방향을 가리킨다. 매처 자체는 정상이다 — 동일 영상과 순수 평행이동에서 정확히 '
     '1.000이 나온다. 그런데 변형을 주면 급격히 떨어진다.')
table(['조건','미뉴셔','정합(inlier)','스코어'],
 [['동일 영상','107','107','1.000'],
  ['평행이동만','107','107','1.000'],
  ['회전 5°','74','33','0.371'],
  ['회전 11° + 노출 변화','72','29','0.330'],
  ['회전 11° + 블러','72','36','0.410'],
  ['복합 열화 전부','24','10','0.153']],
 [103, 50, 50, 50],
 caption='논문 도판의 실촬영본에 변형을 가해 측정. 미뉴셔 수가 줄어드는 것이 점수 하락의 직접 원인이다',
 hl_rows=(5,), mono_cols=(1,2,3))
para('병목은 매칭이 아니라 미뉴셔 재현성이다. 회전 5°만으로 미뉴셔가 107개에서 74개로 준다. '
     '리뷰가 고전 방식의 EER을 43.83 %로 보고한 이유가 이 지점에 있다.')

# =============================================================================
doc.add_page_break()
section_head('05', '실측으로만 드러난 결함')
para('이 프로젝트에서 가장 값이 나간 구간이다. 30개 테스트를 모두 통과한 코드가 실제 사진 앞에서 '
     '무너졌고, 원인은 코드를 읽어서는 보이지 않는 곳에 있었다.')
finding('치명','세그멘테이션 180° 축 모호성',
 'cv::minAreaRect의 각도는 180° 모듈로다. 수학적으로 175.1°와 −4.9°는 같은 축이지만, 그 값을 그대로 '
 '회전각으로 쓰면 손가락이 거의 거꾸로 선다. 그 상태에서 위쪽을 지두로 잘라내면 실제로는 손가락 **밑동**을 '
 '자른다. 해소는 두 단계로 했다. 각도를 (−90°, 90°]로 접고, 남은 상하 반전은 **손가락 끝이 밑동보다 좁다**는 '
 '해부학적 사실로 판별한다.',
 '같은 사진을 5° 회전 → 미뉴셔 20개(기존 11개) · 스코어 0.338(기존 0.013) = 26배')
finding('높음','음영을 융선으로 오인',
 '손가락은 곡면이라 가장자리로 갈수록 어두워지는 완만한 음영이 항상 있다. CLAHE는 국소 히스토그램을 늘릴 뿐 '
 '이 저주파 성분을 제거하지 못한다. 그 결과 Gabor가 융선 대신 **음영의 등고선**을 잡아 매끈한 동심원 곡선을 '
 '만들었다. 밴드패스로 교체해 목표 주기의 1.2배로 흐린 영상을 빼고, 국소 RMS로 나눠 대비를 균일화했다.',
 '실촬영본 미뉴셔 75개(기존 12개) · 일관성 0.994')
finding('높음','세선화가 마스크 경계를 융선으로 추적',
 '이진 영상에서 마스크 경계는 그 자체로 큰 연결영역의 테두리다. Zhang-Suen은 이 테두리를 1픽셀 곡선으로 '
 '그대로 추적한다. 그 결과 손가락 윤곽을 따라가는 가짜 융선이 생기고, 진짜 융선이 거기 T자로 붙으면서 '
 '존재하지 않는 분기점이 대량 생성됐다.',
 '골격 4,315 px(기존 5,459 px) · 윤곽 아티팩트 소멸')
para('세 건 모두 사진을 넣고 돌려보지 않으면 발견되지 않는다. 합성 데이터로 만든 테스트 픽스처는 오히려 '
     '결함을 은폐하고 있었다 — 합성 손가락이 대칭 타원이라 "끝이 좁다"는 판별이 동전던지기가 됐고, 융선이 '
     '끊기지 않아 미뉴셔가 4개뿐이었다. 픽스처를 해부학적으로 맞추고 위상 특이점을 주입한 뒤에야 테스트가 '
     '실제 사진과 같은 조건이 됐다.', before=4)

# =============================================================================
doc.add_page_break()
section_head('06', '논문이 비운 자리를 채운 것')
para('리뷰를 그대로 따라가면 빠지는 것이 있다. 무엇이 빠졌는지 알아보는 것도 논문을 읽는 일의 일부라고 봤다.')
bullets([
 '**표준 생체 지표.** 리뷰가 인용한 연구 상당수가 정확도만 보고한다. 인증 시스템은 개집합 검증이므로 EER·DET·CMC 없이는 성능을 말할 수 없다(ISO/IEC 19795-1). 지표 계산기를 따로 구현하고, 대칭 중첩 분포의 EER이 정확히 1/6이 되는 해석적 기대값으로 검증했다.',
 '**동점 처리.** CMC에서 동점을 유리하게 세면 모든 스코어가 같은 퇴화 매처가 rank-1 100 %를 받는 착시가 생긴다. 동점을 불리하게 세도록 구현하고 그 착시를 테스트로 고정했다.',
 '**범위 밖의 명시.** 위조 방지(PAD, ISO/IEC 30107-3)와 템플릿 보호(ISO/IEC 24745)는 이 단계에서 구현하지 않았다. 구현하지 않았다는 사실과 그 이유를 문서에 남기는 편이, 목록만 채우는 것보다 정직하다고 판단했다.',
 '**생체정보 취급.** 원본 비저장을 기본으로 두고, 저장소에 지문 원본이 커밋되지 않도록 차단했다. 검증용 샘플은 구현 대상 논문에 이미 게재된 도판에서 추출했다.',
])

section_head('07', '판단이 필요했던 지점')
para('함수 하나 때문에 의존성 그래프가 번졌다', 11.5, True, INK, after=3)
para('세선화를 위해 OpenCV contrib의 ximgproc::thinning 하나를 썼다. 그 하나가 contrib → dnn → protobuf → '
     'abseil로 이어져 Windows 빌드가 abseil에서 실패했다. 쓰지도 않는 dnn 때문이었다. Zhang-Suen을 직접 '
     '구현하고 OpenCV 구현과 대조 검증했다.')
table(['입력','OpenCV','자체','불일치'],
 [['사각형','60 px','60 px','0'],['원(고리)','438 px','438 px','0'],
  ['평행선(융선 모사)','965 px','965 px','0'],['Y자 분기','237 px','237 px','0'],
  ['실제 지문 이진화','4,389 px','4,369 px','20 (0.46 %)']],
 [103, 50, 50, 50],
 caption='자체 Zhang-Suen 구현과 OpenCV ximgproc::thinning의 픽셀 단위 대조', mono_cols=(1,2,3))
para('구조적 입력에서는 픽셀 단위로 완전히 일치한다. 차이는 경계 처리 규약에서 나온다 — OpenCV는 테두리를 '
     '세선화하지 않고, 자체 구현은 바깥을 배경으로 보고 처리한다. 지문 ROI는 마스크로 경계에서 떨어져 있어 '
     '실질 영향이 없다. 결과적으로 OpenCV 의존이 core·imgproc·imgcodecs로 줄고 빌드 시간이 30~40분에서 '
     '5~15분이 됐다.')
para('OpenCV 5.0 API 이동은 추측하지 않고 확인했다', 11.5, True, INK, before=8, after=3)
para('MSVC에서 contourArea·minAreaRect·getRotationMatrix2D가 "cv의 멤버가 아니다"로 실패했다. 헤더는 '
     '정상적으로 읽히는데 선언이 없었다. OpenCV 5.x 브랜치 헤더를 받아 코드가 쓰는 API 33개의 소속 모듈을 '
     '전수 확인했다. 이동한 것은 정확히 3개, 새로 생긴 geometry 모듈로 갔고, 영향받는 파일도 하나뿐이었다. '
     '버전 분기를 한 헤더에 모아 4.x와 5.x를 모두 지원하도록 했다.')

# =============================================================================
doc.add_page_break()
section_head('08', '검증 체계')
para('개발 환경은 Linux, 타깃은 Windows다. 이 간극을 메우기 위해 세 축으로 검증했다.')
measures([('Linux 빌드 · 테스트','30 / 30'),('OpenCV 5.x 헤더 문법','16 / 16'),
          ('Windows 크로스컴파일','18 / 18')])
bullets([
 '**OpenCV 의존과 비의존을 분리했다.** 매칭·평가·해설은 OpenCV에 의존하지 않는다. 덕분에 OpenCV 설치 전에도 그 로직을 먼저 검증할 수 있었고, CI가 가벼워진다.',
 '**실패를 예외가 아니라 값으로 다뤘다.** "손가락이 안 보인다"는 버그가 아니라 정상 상태다. 초당 30프레임 루프에서 예외를 던지면 비용과 try/catch 부담이 계속 발생한다.',
 '**인터페이스를 먼저 고정했다.** 결함 수정 세 건이 모두 구현체 내부에서 끝났고 파이프라인 코드는 거의 그대로 유지됐다. 고전 경로와 딥 경로가 같은 계약을 구현하므로 조립부만 바꿔 A/B 비교가 된다.',
])
table(['단계','소요','단계','소요'],
 [['획득','12.2 ms','향상 4단계','58.8 ms'],
  ['품질 게이트','48.0 ms','미뉴셔 추출','2.3 ms'],
  ['분할 · ROI 정규화','49.8 ms','합계','≈ 230 ms']],
 [70, 56, 70, 57],
 caption='단계별 소요 시간. 실촬영본 684×1059, Release, 단일 코어', mono_cols=(1,3))

section_head('09', '한계')
para('성과만큼 한계도 분명히 둔다. 심사에서 이 부분을 먼저 확인하실 것으로 본다.')
bullets([
 '**EER을 산출하지 못했다.** 피험자 1명, 손가락 1개다. 계산기는 구현했고 해석적 기대값으로 검증했지만, 통계적 의미를 가지려면 공개 데이터셋(PolyU 등) 확보가 선결이다.',
 '**"재촬영"은 모사다.** 물리적으로 두 번 촬영한 것이 아니라 원본에 회전·이동·노출·블러·잡음을 가한 것이다. 실제 재촬영은 3D 자세 변화와 피부 변형까지 포함하므로 더 어렵다.',
 '**촬영 품질이 병목이다.** 실측 선명도가 4.0 수준으로, 우리 자신의 품질 게이트가 불합격 판정을 내린다. 융선이 표본화 한계 근처라 회전 5°만으로 미뉴셔가 30 % 줄었다. 근본 개선은 알고리즘이 아니라 획득 쪽에 있다.',
 '**딥 경로는 미구현이다.** 인터페이스와 검증 체계는 갖췄으나 U-Net 세그멘테이션과 MinuNet은 아직 기준선을 대체하지 못했다. 리뷰가 보고한 격차(94.10 % 대 89.61 %)를 직접 재현하는 것이 다음 단계다.',
])

section_head('10', '확인하실 수 있는 것')
para('주장을 문서로만 남기지 않았다. 앱을 한 번 실행하면 각 단계가 어느 논문 절을 구현한 것인지, 원문이 '
     '무엇이라고 했는지, 이번 입력에서 실제로 어떤 수치가 나왔는지를 함께 출력하고 HTML·PDF 보고서로 저장한다.')
d = doc.add_paragraph()
d.paragraph_format.left_indent = Mm(4); d.paragraph_format.space_after = Pt(8)
pPr = d._p.get_or_add_pPr()
sh = OxmlElement('w:shd'); sh.set(qn('w:val'),'clear'); sh.set(qn('w:fill'),'EEF2F4'); pPr.append(sh)
set_font(d.add_run('  [5] 융선 방향장 · 논문 근거 §4 Table 3 (원저 Labati 2010)\n'
                   '      이번 실행 → 일관성 0.989 · 대조 결론 → Gabor 튜닝의 전제 충족  '), MONO, 9.5, False, INK)
bullets([
 '저장소에 논문 원문 PDF, markitdown 변환본, 구현 기술 리서치, 개발 계획서, 코드리뷰가 모두 들어 있다.',
 '코드리뷰 문서는 발견한 결함 17건을 심각도와 함께 공개하고, 그중 무엇을 고쳤고 무엇을 백로그로 남겼는지 명시한다.',
 '빌드는 스크립트 한 줄이다. OpenCV 프리빌트가 있으면 자동 탐색해 vcpkg 단계를 건너뛴다.',
])

rule('12181C', 12)
para('Chowdhury, A. M. M. & Imtiaz, M. H. (2022). Contactless Fingerprint Recognition Using Deep '
     'Learning—A Systematic Review. Journal of Cybersecurity and Privacy, 2(3), 714–730. CC BY 4.0',
     8.5, color=MUTED, after=2)
para('구현 저장소 · github.com/MacTechIN/QingHwa_UNIV_Contactless_fingerprint_Analysis_testing',
     8.5, color=MUTED, after=2)
para('Sam LEE · hotnewton@korea.ac.kr', 8.5, color=MUTED)

out = 'docs/03_chowdhury2022_dl_review/발표자료_비접촉지문인식_논문구현.docx'
doc.save(out)
print('저장:', out)
