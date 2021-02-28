package main

import (
	"encoding/csv"
	"encoding/xml"
	"flag"
	"fmt"
	"io/ioutil"
	"log"
	"os"
	"path/filepath"
	"strconv"
	"strings"
)

var (
	fIn  string
	fOut string
)

func init() {
	flag.StringVar(&fIn, "i", "", "input file path")
	flag.StringVar(&fOut, "o", "", "output file path")
}

type TilesetImage struct {
	XMLName xml.Name `xml:"image"`
	Source  string   `xml:"source,attr"`
}

type TilesetProperty struct {
	XMLName xml.Name `xml:"property"`
	Value   string   `xml:"value,attr"`
}

type TilesetTile struct {
	XMLName    xml.Name          `xml:"tile"`
	ID         int               `xml:"id,attr"`
	Image      TilesetImage      `xml:"image"`
	Properties []TilesetProperty `xml:"properties>property"`
}

type Tileset struct {
	XMLName  xml.Name      `xml:"tileset"`
	Tiles    []TilesetTile `xml:"tile"`
	TilesMap map[int]string
}

type MapTileset struct {
	XMLName xml.Name `xml:"tileset"`
	Source  string   `xml:"source,attr"`
}

type MapLayer struct {
	XMLName xml.Name `xml:"layer"`
	ID      int      `xml:"id,attr"`
	Data    string   `xml:"data"`
	Records [][]string
}

type Map struct {
	XMLName xml.Name   `xml:"map"`
	Width   int        `xml:"width,attr"`
	Height  int        `xml:"height,attr"`
	Tileset MapTileset `xml:"tileset"`
	Layers  []MapLayer `xml:"layer"`
}

func generateTextMap(tmx *Map, tsx *Tileset) (string, error) {
	var txt strings.Builder
	fmt.Fprintf(&txt, "%dx%d\n", tmx.Height, tmx.Width)
	fmt.Fprintf(&txt, "%02d\n", len(tmx.Layers))
	for _, layer := range tmx.Layers {
		for _, record := range layer.Records {
			for _, col := range record {
				i, err := strconv.Atoi(col)
				if err != nil {
					return "", err
				}
				if c, ok := tsx.TilesMap[i]; ok {
					fmt.Fprintf(&txt, "%s", c)
				} else {
					return "", fmt.Errorf("unknown tile id:%d", i)
				}
			}
			fmt.Fprintf(&txt, "%c", '\n')
		}
	}
	return txt.String(), nil
}

func parseTileset(fIn string) (*Tileset, error) {
	xmlFile, err := os.Open(fIn)
	if err != nil {
		return nil, err
	}
	defer xmlFile.Close()

	byteVal, _ := ioutil.ReadAll(xmlFile)

	var tsx Tileset
	tsx.TilesMap = make(map[int]string)
	if err := xml.Unmarshal(byteVal, &tsx); err != nil {
		return nil, err
	}

	tsx.TilesMap[0] = " "
	for _, tile := range tsx.Tiles {
		tsx.TilesMap[tile.ID+1] = tile.Properties[0].Value
	}
	return &tsx, nil
}

func parseMap(fIn string) (*Map, error) {
	xmlFile, err := os.Open(fIn)
	if err != nil {
		return nil, err
	}
	defer xmlFile.Close()

	byteVal, _ := ioutil.ReadAll(xmlFile)

	var tmx Map

	if err := xml.Unmarshal(byteVal, &tmx); err != nil {
		return nil, err
	}

	for i := range tmx.Layers {
		r := csv.NewReader(strings.NewReader(strings.Replace(tmx.Layers[i].Data, ",\n", "\n", -1)))

		tmx.Layers[i].Records, err = r.ReadAll()
		if err != nil {
			return nil, err
		}
	}
	return &tmx, nil
}

func main() {

	flag.Parse()
	if fIn == "" {
		log.Fatal("-i is required")
	}

	tmx, err := parseMap(fIn)
	if err != nil {
		log.Fatal(err)
	}

	tsx, err := parseTileset(filepath.Join(filepath.Dir(fIn), tmx.Tileset.Source))
	if err != nil {
		log.Fatal(err)
	}

	txt, err := generateTextMap(tmx, tsx)
	if err != nil {
		log.Fatal(err)
	}

	if fOut != "" {
		f, err := os.Create(fOut)
		if err != nil {
			log.Fatal(err)
		}
		defer f.Close()

		_, err = f.WriteString(txt)
		if err != nil {
			log.Fatal(err)
		}
	} else {
		fmt.Println(txt)
	}
}
