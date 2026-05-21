export default function RaspberryCard() {
  return (
    <div className="device-card">
      <h2>Raspberry Pi</h2>
      <p>Status: <span className="online">Online</span></p>
      <p>Temperatura: 42°C</p>
      <button className="device-btn">Ver detalhes</button>
    </div>
  );
}
