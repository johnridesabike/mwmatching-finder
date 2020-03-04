const d3 = require("d3");

const drag = simulation => {
  
  function dragstarted(d) {
    if (!d3.event.active) simulation.alphaTarget(0.3).restart();
    d.fx = d.x;
    d.fy = d.y;
  }
  
  function dragged(d) {
    d.fx = d3.event.x;
    d.fy = d3.event.y;
  }
  
  function dragended(d) {
    if (!d3.event.active) simulation.alphaTarget(0);
    d.fx = null;
    d.fy = null;
  }
  
  return d3.drag()
      .on("start", dragstarted)
      .on("drag", dragged)
      .on("end", dragended);
}

/*
const color = d => d3.scaleOrdinal(d3.schemeCategory10)(d.group);
const color = {
  const scale = d3.scaleOrdinal(d3.schemeCategory10);
  return d => scale(d.group);
}
*/

const height = 400
const width = 400

const data = require("./miserables.js");

const chart = () => {
  const links = data.links;
  const nodes = data.nodes.map(d => ({"data": {"id": d}}));

  const simulation = d3
      .forceSimulation(nodes)
      .force("link", d3.forceLink(links).id(d => d.data.id))
      .force("charge", d3.forceManyBody())
      .force("center", d3.forceCenter(width / 2, height / 2));

  const svg = d3.select(document.querySelector("#graph-test"))
      .attr("viewBox", [0, 0, width, height]);

  const link = svg.append("g")
      .attr("stroke", "#999")
      .attr("stroke-opacity", 0.6)
    .selectAll("line")
    .data(links)
    .join("line")
      .attr("stroke-width", d => Math.sqrt(d.weight));

  const node = svg.append("g")
      .attr("stroke", "#fff")
      .attr("stroke-width", 1.5)
    .selectAll("circle")
    .data(nodes)
    .join("circle")
      .attr("r", 5)
      //.attr("fill", color)
      .call(drag(simulation));

  node.append("title")
      .text(d => d.data.id);

  node.append("text").text(d => d.data.id).attr("style", "color:blue");

  simulation.on("tick", () => {
    link
        .attr("x1", d => d.source.x)
        .attr("y1", d => d.source.y)
        .attr("x2", d => d.target.x)
        .attr("y2", d => d.target.y);

    node
        .attr("cx", d => d.x)
        .attr("cy", d => d.y);
  });

  // invalidation.then(() => simulation.stop());

  return svg.node();
}

chart();
