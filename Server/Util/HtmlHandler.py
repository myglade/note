import HTMLParser
import html2text
import urllib

class HtmlHandler(HTMLParser):
        
    def handle_starttag(self, tag, attrs):
        if tag != 'img':
            return

        print("Start tag:", tag)
        for attr in attrs:
            print("     attr:", attr)
            if attr[0] == 'src':
                url = attr[1]
                if not url.startswith('http'):
                    url += 'http://'

                self.images[url] = ""

        '''
     attr: ('src', 'python-logo.png')
     attr: ('alt', 'The Python logo')

        '''

    def feed(self, str):
        self.tempPath = "\\"
        self.images = {}
        self.feed(str)
        self.text = html2text.html2text(str)

        self.downloadImage(self.images)

    def downloadImage(self, images):
        #urllib.urlretrieve("http://www.gunnerkrigg.com//comics/00000001.jpg", "00000001.jpg")
        for url in images.keys():
            pos = url.rfind("/")
            localPath = self.tempPath + url[pos+1:]
         
            urllib.urlretrieve(url, localPath)
            self.images[url] = localPath

parser = MyHTMLParser()


