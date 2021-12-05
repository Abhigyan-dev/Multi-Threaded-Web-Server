import random

no_of_files = 100
factor = 16

labelIds = [i for i in range(no_of_files)]
random.shuffle(labelIds)

for count, i in enumerate(labelIds):

    f = open("%d.html"%(i), "a")  # append mode

    f.write("<html>\n")

    f.write("<head>\n")
    f.write("<title>Arts and Crafts</title>\n")
    f.write("<link rel=\"stylesheet\" href=\"./resources/index.css\" type=\"text/css\">\n")
    f.write("</head>\n")

    f.write("<body>\n")

    f.write("<header>\n")
    f.write("<h1>Dasmoto's Arts & Crafts</h1>\n")
    f.write("</header>\n")

    f.write("<main>\n")
    for j in range(factor*count):
        f.write("<section>\n")
        f.write("<h3 class=\"top\" id=\"top1\">Brushes</h3>\n")
        f.write("<img src=\"./resources/images/I2.jpeg\" alt=\"Paint Brushes\">\n")
        f.write("<h3 class=\"down\">Hacksaw Brushes</h3>\n")
        f.write("<p>Made of the highest quality oak, Hacksaw brushes are known for their weight and ability to hold paint in large amounts. Available in different sizes.<span class=\"blueB\">Starting at $3.00 / brush</span></p>\n")
        f.write("</section>\n")
    f.write("</main>\n")

    f.write("</body>\n")

    f.write("</html>\n")

