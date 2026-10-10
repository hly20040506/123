import json,re,glob,sys
import pypdf
W='/home/user/123/work2'
L=json.load(open(f'{W}/list.json'))
out={}
for i,x in enumerate(L,1):
    n=x['pdf'].rsplit('/',1)[1][:-4]
    pages=open(f"{W}/txt/{x['id']}_{n}.txt",errors='ignore').read().split('\f')
    ref=len(pages)
    for pi,p in enumerate(pages,1):
        if pi>3 and re.search(r'^\s*References\s*$',p,re.M):
            ref=pi;break
    found={}
    # text links (allow "https: //", wrapped lines)
    for pi,p in enumerate(pages,1):
        flat=re.sub(r'\s+',' ',p)
        for m in re.finditer(r'(?:https?:\s?//\s?)?(?:www\.)?(github\.com|huggingface\.co|gitlab\.com)\s?/\s?[\w.\-]+(?:\s?/\s?[\w.\-]+)?',flat):
            u=re.sub(r'\s','',m.group(0))
            u=re.sub(r'^https?://','',u).rstrip('.')
            found.setdefault(u,set()).add(('t',pi))
    # annotation links
    try:
        r=pypdf.PdfReader(f"/home/user/123/ECCV2026_papers_new_201-383/{x['id']}_{n}.pdf")
        for pi,p in enumerate(r.pages,1):
            for a in p.get('/Annots') or []:
                a=a.get_object(); u=a.get('/A',{}).get('/URI') if a.get('/A') else None
                if u and re.search(r'github\.com|huggingface\.co|gitlab\.com',u):
                    u=re.sub(r'^https?://','',u).rstrip('/')
                    found.setdefault(u,set()).add(('a',pi))
    except Exception as e:
        print('ERR',i,e,file=sys.stderr)
    out[i]=dict(ref=ref,links={u:sorted(v) for u,v in found.items()})
json.dump(out,open(f'{W}/links.json','w'),indent=0)
print('done')
